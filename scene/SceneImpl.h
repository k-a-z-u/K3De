#ifndef SCENEIMPL_H
#define SCENEIMPL_H

#include "Scene.h"

#include "WaterRenderer.h"
#include "ShadowRenderer.h"
#include "ShadowRendererSimple.h"
#include "PostProcessRenderer.h"

#include "../world/Water.h"
#include "../world/Terrain.h"
#include "../world/Skybox.h"

const ScreenSize& Scene::getScreenSize() const {
	return size;
}

ScreenSize* Scene::getScreenSizePtr() {
	return &size;
}

void Scene::add(IRenderable* r) {
	if (dynamic_cast<Water*>(r)) {throw Exception("must not add water via add()");}
	renderables.push_back(r);
}

void Scene::addWater(Water* w) {
	if (!waterRenderer) {throw Exception("set the water-renderer first!");}
//	w->setReflect(waterRenderer->texReflect);
//	w->setRefract(waterRenderer->texRefract);
	waters.push_back(w);
}

void Scene::setup() {
	clipZ = Vec2(-9999,+9999);

	// default camera
	cam.setPosition(2, 2, 2);
	cam.setLookAt(0,0,0);
	cam.setUp(0,1,0);
	cam.setFOV(45);

	// default lights
	//setNumLights(1);
	getLight(0).setColor(1,1,1);
	getLight(0).setPosition(4,4,4);
	getLight(0).setActive(true);
	getLight(0).setCastsShadows(true);

	overwriteShader = nullptr;
}

void Scene::setEnableShadows(bool enable) {
	Assert::isTrue(renderables.empty(), "must call setEnableShadows() before adding renderables!");
	if(enable) {
		if (!shadowRenderer) {shadowRenderer = new ShadowRendererSimple(this);}
	} else {
		if (shadowRenderer) {delete shadowRenderer; shadowRenderer = nullptr;}
	}
}

void Scene::setEnableWater(bool enable) {
	Assert::isTrue(renderables.empty(), "must call setEnableWater() before adding renderables!");
	if(enable) {
		if (!waterRenderer) {waterRenderer = new WaterRenderer(this);}
	} else {
		if (waterRenderer) {delete waterRenderer; waterRenderer = nullptr;}
	}
}

void Scene::resize(const ScreenSize& size) {
	this->size = size;
	const int w = size.viewport.width;
	const int h = size.viewport.height;
	getCamera().setViewportSize(w, h);
	if (getShadowRenderer())		{getShadowRenderer()->resize(w,h);}
	if (getWaterRenderer())			{getWaterRenderer()->resize(w,h);}
	if (getPostProcessRenderer())	{getPostProcessRenderer()->resize(w,h);}
}

void Scene::render() {

	// timing update
	ss.lastRenderStart = ss.renderStart;
	ss.renderStart = Time::runtime();

	// update animations
	for (size_t i = 0; i < animations.size(); ++i) {
		IAnimation* ani = animations[i];
		ani->step(this, ss);
		if (ani->isDone()) {
			animations.erase(animations.begin() + i);
			delete ani;
			--i;
		}
	}

	// inform the scene
	onBeforeRender(ss);

	// inform every renderable about the pending render process
	for (IRenderable* r : renderables) {r->onBeforeRender(ss);}
	for (IRenderable* r : ui.elements) {r->onBeforeRender(ss);}

	// update the light-uniform
	lighting.upload();
	//uboLights.bind();

	// other params
	rs.screenWidht = getScreenSize().viewport.width;
	rs.screenHeight = getScreenSize().viewport.height;


	if (waterRenderer)	{waterRenderer->update();}

	if (shadowRenderer)	{shadowRenderer->update();}

	if (postProcRenderer) {postProcRenderer->begin3D();}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		getCamera().setViewportSize(getScreenSize().viewport.width, getScreenSize().viewport.height);

		renderForNormal();
		renderUI();

	if (postProcRenderer) {
		postProcRenderer->end3D();
		// TODO not needed?
		{
			rs.matrices.V = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
			rs.matrices.P = Mat4::identity();
			rs.matrices.PV = rs.matrices.P * rs.matrices.V;
		}

		postProcRenderer->showResult(ss, rs);

	}

	// time it took to render the current frame
	ss.lastRenderDuration = Time::runtime() - ss.renderStart;
	fps.rendered();

}

void Scene::renderThis(IRenderable* r, RenderState& rs) {

	// skip disabled elements
	if (!r->isEnabled()) {return;}

	// skip invisible elements
	//rs.matrices.PV = cam.getPVMatrix();
	//rs.matrices.VM = cam.getVMatrix() * r->getMatrix();
	rs.matrices.PVM = rs.matrices.P * rs.matrices.V * r->getMatrix();

	if (!r->isVisible(rs.matrices.PVM)) {return;}

	// get shader and perform sanity check
	Shader* s = (overwriteShader) ? (overwriteShader) : (r->getShader());
	_assertNotNull(s, "shader is null");

	// activate the shader
	s->bind();

	// configure variable parameters [changing almost every frame]
	//if (s->hasUniform("clipY"))			{s->setVector("clipY", clipY);}
	if (s->hasUniform(DefaultUniform::CLIPPING))						{s->setVector(DefaultUniform::CLIPPING, clipZ);}
	//if (s->hasUniform("camPos"))		{s->setVector("camPos", cam.getPosition());}
	if (s->hasUniform(DefaultUniform::CAMERA_POSITION))					{s->setVector(DefaultUniform::CAMERA_POSITION, cam.getPosition());}
	//if (s->hasUniform("M"))				{s->setMatrix("M", r->getMatrix());}
	if (s->hasUniform(DefaultUniform::MODEL_MATRIX))					{s->setMatrix(DefaultUniform::MODEL_MATRIX, r->getMatrix());}
	//if (s->hasUniform("V"))				{s->setMatrix("V", rs.matrices.V);}//cam.getVMatrix());}
	if (s->hasUniform(DefaultUniform::VIEW_MATRIX))						{s->setMatrix(DefaultUniform::VIEW_MATRIX, rs.matrices.V);}//cam.getVMatrix());}
	//if (s->hasUniform("P"))				{s->setMatrix("P", rs.matrices.P);}//cam.getPMatrix());}
	if (s->hasUniform(DefaultUniform::PROJECTION_MATRIX))				{s->setMatrix(DefaultUniform::PROJECTION_MATRIX, rs.matrices.P);}//cam.getPMatrix());}
	//if (s->hasUniform("PV"))			{s->setMatrix("PV", rs.matrices.PV);}
	if (s->hasUniform(DefaultUniform::VIEW_PROJECTION_MATRIX))			{s->setMatrix(DefaultUniform::VIEW_PROJECTION_MATRIX, rs.matrices.PV);}
	//if (s->hasUniform("PVM"))			{s->setMatrix("PVM", rs.matrices.PVM);}
	if (s->hasUniform(DefaultUniform::MODEL_VIEW_PROJECTION_MATRIX))	{s->setMatrix(DefaultUniform::MODEL_VIEW_PROJECTION_MATRIX, rs.matrices.PVM);}
//		if (s->hasUniform("PVshadow"))		{s->setMatrix("PVshadow", shadowPV);}
	//if (s->hasUniform("time"))			{s->setFloat("time", ss.getCurrentTime().seconds());}
	if (s->hasUniform(DefaultUniform::TIME))							{s->setFloat(DefaultUniform::TIME, ss.getCurrentTime().seconds());}

	// configure fixed parameters once [constant between frames]
	if (s->hasUniformBlock("Lights"))	{s->setVarOnce("Lights", lighting.getUBO());}
	//if (s->hasUniform("screenWidth"))	{s->setFloatOnce("screenWidth", rs.screenWidht);}
	if (s->hasUniform(DefaultUniform::SCREEN_WIDTH))	{s->setFloatOnce(DefaultUniform::SCREEN_WIDTH, rs.screenWidht);}
	//if (s->hasUniform("screenHeight"))	{s->setFloatOnce("screenHeight", rs.screenHeight);}
	if (s->hasUniform(DefaultUniform::SCREEN_HEIGHT))	{s->setFloatOnce(DefaultUniform::SCREEN_HEIGHT, rs.screenHeight);}

	// render the object
	r->render(ss, rs);

	// disable the shader
	s->unbind();

//		HasAABB* m = dynamic_cast<HasAABB*>(r);
//		if (m) {
//			AABBRenderable rr(m);
//			if (s->hasUniform("M"))			{s->setMatrix("M", rr.getMatrix());}
//			rr.render(rs);
//		}

}

void Scene::renderForWater() {

	rs.matrices.P = cam.getPMatrix();
	rs.matrices.V = cam.getVMatrix();
	rs.matrices.PV = cam.getPVMatrix();

	for (IRenderable* r : renderables)	{renderThis(r, rs);}
	for (Terrain* t : terrains)			{renderThis(t, rs);}
	if (skybox)							{renderThis(skybox, rs);}
	for (IRenderable* ps : particles)	{renderThis(ps, rs);}

}

void Scene::renderForShadows() {

	rs.matrices.P = cam.getPMatrix();
	rs.matrices.V = cam.getVMatrix();
	rs.matrices.PV = cam.getPVMatrix();

	for (IRenderable* r : renderables) {
		if (r->castsShadows()) {
			renderThis(r, rs);
		}
	}

	for (Terrain* t : terrains) {
		if (t->castsShadows()) {
			renderThis(t, rs);
		}
	}

}

void Scene::renderForNormal() {

	rs.matrices.P = cam.getPMatrix();
	rs.matrices.V = cam.getVMatrix();
	rs.matrices.PV = cam.getPVMatrix();

	for (IRenderable* r : renderables)	{renderThis(r, rs);}
	for (Water* w : waters)				{renderThis(w, rs);}
	for (Terrain* t : terrains)			{renderThis(t, rs);}
	if (skybox)							{renderThis(skybox, rs);}
	for (IRenderable* ps : particles)	{renderThis(ps, rs);}

}

void Scene::renderUI() {

	// this matrix mirros the y axis (0,0 = upper left)
	// and changes the area from [-1:+1] to [0:1]
	// -> (0,0) = upper left, (1,1) = lower right
	rs.matrices.V = {2,0,0,0, 0,-2.0,0,0, 0,0,1,0, -1.0,+1.0,0,1};
	rs.matrices.P = Mat4::identity();
	rs.matrices.PV = rs.matrices.P * rs.matrices.V;

	// TODO: enable culling to speed things up?? or doesn't this change anything at all?
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	for (Renderable* s : ui.elements)	{renderThis(s, rs);}
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

}

#endif // SCENEIMPL_H

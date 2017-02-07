#ifndef SCENEIMPL_H
#define SCENEIMPL_H

#include "Scene.h"

#include "WaterRenderer.h"
#include "ShadowRenderer.h"
#include "ShadowRendererSimple.h"
#include "PostProcessRenderer.h"

/** add a new water to the scene */
void Scene::addWater(Water* w) {
	w->setReflect(waterRenderer->texReflect);
	w->setRefract(waterRenderer->texRefract);
	waters.push_back(w);
}

void Scene::setup() {
	clipY = Vec2(-9999,+9999);

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
	if(enable) {
		if (!shadowRenderer) {shadowRenderer = new ShadowRendererSimple(this);}
	} else {
		if (shadowRenderer) {delete shadowRenderer; shadowRenderer = nullptr;}
	}
}

void Scene::setEnableWater(bool enable) {
	if(enable) {
		if (!waterRenderer) {waterRenderer = new WaterRenderer(this);}
	} else {
		if (waterRenderer) {delete waterRenderer; waterRenderer = nullptr;}
	}
}

void Scene::render() {

	// timing update
	ss.lastRenderStart = ss.renderStart;
	ss.renderStart = Time::runtime();

	// inform the scene
	onBeforeRender();

	// inform every renderable about the pending render process
	for (Renderable* r : renderables) {r->onBeforeRender(ss);}
	for (Renderable* r : ui.elements) {r->onBeforeRender(ss);}

	// update the light-uniform
	lighting.upload();
	//uboLights.bind();

	// other params
	rs.screenWidht = getScreenSize().width;
	rs.screenHeight = getScreenSize().height;


	if (waterRenderer)	{waterRenderer->update();}

	if (shadowRenderer)	{shadowRenderer->update();}

	if (postProcRenderer) {postProcRenderer->begin3D();}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		getCamera().setScreenSize();

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
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		//renderThis(postProcRenderer, rs);
		postProcRenderer->showResult(ss, rs);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
	}

	// time it took to render the current frame
	ss.lastRenderDuration = Time::runtime() - ss.renderStart;

}

#endif // SCENEIMPL_H

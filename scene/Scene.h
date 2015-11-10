#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "Camera.h"
#include "Light.h"
#include "Renderable.h"

#include "../world/Water.h"
#include "../world/Terrain.h"

#include "../3D/AABBRenderable.h"

#include "../world/Skybox.h"

#include "../mesh/MeshFactory.h"
#include "../shader/ShaderFactory.h"
#include "../textures/TextureFactory.h"

#include "../2D/UI.h"
#include "../2D/SpriteFactory.h"

#include "RenderStage.h"

class ShadowRenderer;
class WaterRenderer;
class ParticleSystem;

class Scene {

protected:

	/** lighting */
	struct Lights {
		LightAttributes light[8];
		int num;
	} lights __attribute__((packed));

	UBO uboLights;

	/** camera */
	Camera cam;

	Vec4 clearColor;


	/** render water textures */
	WaterRenderer* waterRenderer;

	/** render shadows */
	ShadowRenderer* shadowRenderer;

	/** terrain layers */
	std::vector<Terrain*> terrains;

	/** water layers */
	std::vector<Water*> waters;

	std::vector<Renderable*> particles;

	/** "normal" renderables */
	std::vector<Renderable*> renderables;

	/** the sky */
	SkyBox* skybox;

	/** 2D UI renderables */
	UI ui;


	MeshFactory meshFac;
	ShaderFactory shaderFac;
	TextureFactory textureFac;
	SpriteFactory spriteFac;
	Vec2 clipY;

	RenderStage rs;

	Shader* overwriteShader;
	Mat4 shadowPV;

public:

	Scene() : waterRenderer(nullptr), shadowRenderer(nullptr), skybox(nullptr) {
		setup();
	}

	void setup();

	/** configure whether this scene supports shadows */
	void setEnableShadows(bool enable);

	/** configure whether this scene supports water */
	void setEnableWater(bool enable);

	void setClipY(const float min, const float max) {
		clipY.x = min;
		clipY.y = max;
	}



	/** set the number of lights to use */
	void setNumLights(const int num) {
		lights.num = num;
	}

	/** get the light behind the given index */
	Light getLight(const int idx) {
		return Light(&lights.light[idx]);
	}

	/** add the given renderable to the scene */
	void add(Renderable* r) {
		renderables.push_back(r);
	}

	void addParticles(Renderable* ps) {
		particles.push_back(ps);
	}

	/** add a new water to the scene */
	void addWater(Water* w);

	/** add a new terrain to the scene */
	void addTerrain(Terrain* t) {
		terrains.push_back(t);
	}

	/** set the sky */
	void setSkyBox(SkyBox* skybox) {
		this->skybox = skybox;
	}

	/** get the scene's mesh-factory */
	MeshFactory& getMeshFactory() {return meshFac;}

	/** get the scene's texture-factory */
	TextureFactory& getTextureFactory() {return textureFac;}

	/** get the scene's shader-factory */
	ShaderFactory& getShaderFactory() {return shaderFac;}

	/** get the scene's sprite-factory */
	SpriteFactory& getSpriteFactory() {return spriteFac;}

	/** get the scene's camera */
	Camera& getCamera() {return cam;}

	/** get the scene's 2D ui */
	UI& getUI() {return ui;}

	void setOverwriteShader(Shader* shader) {
		this->overwriteShader = shader;
	}

	void setShadowPV(const Mat4& mat) {

		// this bias matrix is used to convert from [-1;+1] to [0;+1]
		// adding this directly to the matrix saves time within the shaders!
		static const float bias[16] = {
			0.5, 0.0, 0.0, 0.5,
			0.0, 0.5, 0.0, 0.5,
			0.0, 0.0, 0.5, 0.5,
			0.0, 0.0, 0.0, 1.0
		};
		static const Mat4 mBias = Mat4::fromRawRowMajor(bias);

		this->shadowPV = mBias * mat;

	}

	/** get the scene's shadow renderer */
	ShadowRenderer* getShadowRenderer() {return shadowRenderer;}


	/** scene will be rendered soon */
	virtual void onBeforeRender() = 0;

	void setClearColor(const Vec4& color) {
		this->clearColor = color;
		glClearColor(color.r, color.g, color.b, color.a);
	}

	const Vec4& getClearColor() {return clearColor;}

private:

	friend class Engine;
	friend class WaterRenderer;
	friend class ShadowRenderer;
	friend class ShadowRendererSimple;

	/** render the complete scene. requires multiple passes */
	void render();

	/** render everything visible to water reflection and refraction */
	void renderForWater() {
		for (Renderable* r : renderables)	{renderThis(r, rs);}
		for (Terrain* t : terrains)			{renderThis(t, rs);}
		if (skybox)							{renderThis(skybox, rs);}
		for (Renderable* ps : particles)	{renderThis(ps, rs);}
	}

public:

	/** render everything casting a shadow */
	void renderForShadows() {
		for (Renderable* r : renderables)	{
			renderThis(r, rs);	// TODO remove elements not casting a shadow
		}
		for (Terrain* t : terrains)			{renderThis(t, rs);}
	}

private:

	/** render everything for the normal pass */
	void renderForNormal() {
		rs.matrices.PV = cam.getPVMatrix();
		for (Renderable* s : ui.elements)	{renderThis(s, rs);}
		for (Renderable* r : renderables)	{renderThis(r, rs);}
		for (Water* w : waters)				{renderThis(w, rs);}
		for (Terrain* t : terrains)			{renderThis(t, rs);}
		if (skybox)							{renderThis(skybox, rs);}
		for (Renderable* ps : particles)	{renderThis(ps, rs);}
	}

	/** render one object (if enabled, visible, ...) */
	void renderThis(Renderable* r, RenderStage& rs) {

		// skip disabled elements
		if (!r->isEnabled()) {return;}

		// skip invisible elements
		rs.matrices.PV = cam.getPVMatrix();
		rs.matrices.VM = cam.getVMatrix() * r->getMatrix();
		rs.matrices.PVM = cam.getPMatrix() * cam.getVMatrix() * r->getMatrix();
		if (!r->isVisible(rs.matrices.PVM)) {return;}

		// get shader and perform sanity check
		Shader* s = (overwriteShader) ? (overwriteShader) : (r->getShader());
		_assertNotNull(s, "shader is null");

		// activate the shader
		s->bind();

		// configure default parameters
		if (s->hasUniform("clipY"))			{s->setVector("clipY", clipY);}
		if (s->hasUniform("camPos"))		{s->setVector("camPos", cam.getPosition());}
		if (s->hasUniform("M"))				{s->setMatrix("M", r->getMatrix());}
		if (s->hasUniform("V"))				{s->setMatrix("V", cam.getVMatrix());}
		if (s->hasUniform("P"))				{s->setMatrix("P", cam.getPMatrix());}
		if (s->hasUniform("PV"))			{s->setMatrix("PV", rs.matrices.PV);}
		if (s->hasUniform("PVM"))			{s->setMatrix("PVM", rs.matrices.PVM);}
		if (s->hasUniform("PVshadow"))		{s->setMatrix("PVshadow", shadowPV);}
		if (s->hasUniformBlock("Lights"))	{s->setVar("Lights", uboLights);}
		if (s->hasUniform("time"))			{s->setFloat("time", Engine::get()->getTimeSec());}
		r->render(rs);

//		HasAABB* m = dynamic_cast<HasAABB*>(r);
//		if (m) {
//			AABBRenderable rr(m);
//			if (s->hasUniform("M"))			{s->setMatrix("M", rr.getMatrix());}
//			rr.render(rs);
//		}

	}

};

#include "WaterRenderer.h"
#include "ShadowRenderer.h"
#include "ShadowRendererSimple.h"
#include "ShadowRendererESM.h"

/** add a new water to the scene */
void Scene::addWater(Water* w) {
	w->setReflect(waterRenderer->texReflect);
	w->setRefract(waterRenderer->texRefract);
	waters.push_back(w);
}

void Scene::setup() {
	clipY = Vec2(-9999,+9999);
	cam.setPosition(2, 2, 2);
	cam.setLookAt(0,0,0);
	cam.setUp(0,1,0);
	cam.setFOV(45);
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

	onBeforeRender();

	// update the light-uniform
	uboLights.upload((uint8_t*)&lights, sizeof(lights));
	uboLights.bind();

	if (waterRenderer)	{waterRenderer->update();}
	if (shadowRenderer)	{shadowRenderer->update();}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	getCamera().setScreenSize();

	renderForNormal();

}

#endif // SCENE_H

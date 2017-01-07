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

#include "RenderState.h"
#include "SceneState.h"

class ShadowRenderer;
class WaterRenderer;
class PostProcessRenderer;
class ParticleSystem;

/**
 * @brief a scene is something that describes a render-setup
 * including light, camera, objects, etc.
 *
 * this is the base-class for all scenes.
 * the user has to subclass thise one and implement the missing methods
 */
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

	/** render shadow maps */
	ShadowRenderer* shadowRenderer;

	/** render to texture for post-processing */
	PostProcessRenderer* postProcRenderer;


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

	RenderState rs;
	SceneState ss;

	Shader* overwriteShader;
	Mat4 shadowPV;

public:

	Scene() : waterRenderer(nullptr), shadowRenderer(nullptr), postProcRenderer(nullptr), skybox(nullptr) {
		setup();
	}

	/** init with default parameters */
	inline void setup();

	/** configure whether this scene supports shadows */
	inline void setEnableShadows(bool enable);

	/** configure whether this scene supports water */
	inline void setEnableWater(bool enable);

	/** configure the post-processing renderer to use (if any) */
	void setPostProcRenderer(PostProcessRenderer* ppRenderer) {
		this->postProcRenderer = ppRenderer;
	}

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
	inline void addWater(Water* w);

	/** add a new terrain to the scene */
	void addTerrain(Terrain* t) {
		terrains.push_back(t);
	}

	/** set the sky */
	void setSkyBox(SkyBox* skybox) {
		this->skybox = skybox;
	}

	/** get the current screen size */
	ScreenSize getScreenSize() const {
		return Engine::get()->getScreenSize();
	}

	/** get the current screen size */
	const ScreenSize* getScreenSizePtr() const {
		static ScreenSize screen;					// TODO: fix
		screen = Engine::get()->getScreenSize();
		return &screen;
	}

	/** get current FPS, given by the last render-time */
	float getFPS() const {
		return ss.getFPS();
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
	friend class PostProcessRenderer;
	friend class ShadowRenderer;
	friend class ShadowRendererSimple;

	/** render the complete scene. requires multiple passes */
	inline void render();

	/** render everything visible to water reflection and refraction */
	void renderForWater() {

		rs.matrices.P = cam.getPMatrix();
		rs.matrices.V = cam.getVMatrix();
		rs.matrices.PV = cam.getPVMatrix();

		for (Renderable* r : renderables)	{renderThis(r, rs);}
		for (Terrain* t : terrains)			{renderThis(t, rs);}
		if (skybox)							{renderThis(skybox, rs);}
		for (Renderable* ps : particles)	{renderThis(ps, rs);}

	}

public:

	/** render everything casting a shadow */
	void renderForShadows() {

		rs.matrices.P = cam.getPMatrix();
		rs.matrices.V = cam.getVMatrix();
		rs.matrices.PV = cam.getPVMatrix();

		for (Renderable* r : renderables)	{
			renderThis(r, rs);	// TODO remove elements not casting a shadow
		}
		for (Terrain* t : terrains)			{renderThis(t, rs);}

	}

private:

	/** render everything for the normal pass */
	void renderForNormal() {

		rs.matrices.P = cam.getPMatrix();
		rs.matrices.V = cam.getVMatrix();
		rs.matrices.PV = cam.getPVMatrix();

		for (Renderable* r : renderables)	{renderThis(r, rs);}
		for (Water* w : waters)				{renderThis(w, rs);}
		for (Terrain* t : terrains)			{renderThis(t, rs);}
		if (skybox)							{renderThis(skybox, rs);}
		for (Renderable* ps : particles)	{renderThis(ps, rs);}

	}

	void renderUI() {

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

	/** render one object (if enabled, visible, ...) */
	void renderThis(Renderable* r, RenderState& rs) {

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
		if (s->hasUniform("clipY"))			{s->setVector("clipY", clipY);}
		if (s->hasUniform("camPos"))		{s->setVector("camPos", cam.getPosition());}
		if (s->hasUniform("M"))				{s->setMatrix("M", r->getMatrix());}
		if (s->hasUniform("V"))				{s->setMatrix("V", rs.matrices.V);}//cam.getVMatrix());}
		if (s->hasUniform("P"))				{s->setMatrix("P", rs.matrices.P);}//cam.getPMatrix());}
		if (s->hasUniform("PV"))			{s->setMatrix("PV", rs.matrices.PV);}
		if (s->hasUniform("PVM"))			{s->setMatrix("PVM", rs.matrices.PVM);}
		if (s->hasUniform("PVshadow"))		{s->setMatrix("PVshadow", shadowPV);}
		if (s->hasUniform("time"))			{s->setFloat("time", ss.getCurrentTime().seconds());}

		// configure fixed parameters once [constant between frames]
		if (s->hasUniformBlock("Lights"))	{s->setVarOnce("Lights", uboLights);}
		if (s->hasUniform("screenWidth"))	{s->setFloatOnce("screenWidth", getScreenSize().width);}
		if (s->hasUniform("screenHeight"))	{s->setFloatOnce("screenHeight", getScreenSize().height);}

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

};

#include "WaterRenderer.h"
#include "ShadowRenderer.h"
#include "ShadowRendererSimple.h"
#include "PostProcessRenderer.h"
//#include "ShadowRendererESM.h"

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

	// timing update
	ss.lastRenderStart = ss.renderStart;
	ss.renderStart = Time::runtime();

	// inform the scene
	onBeforeRender();

	// inform every renderable about the pending render process
	for (Renderable* r : renderables) {r->onBeforeRender(ss);}
	for (Renderable* r : ui.elements) {r->onBeforeRender(ss);}

	// update the light-uniform
	uboLights.upload((uint8_t*)&lights, sizeof(lights));
	uboLights.bind();


	if (waterRenderer)	{waterRenderer->update();}

	if (shadowRenderer)	{shadowRenderer->update();}

	if (postProcRenderer) {postProcRenderer->begin();}

	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		getCamera().setScreenSize();

		renderForNormal();
		renderUI();

	if (postProcRenderer) {
		postProcRenderer->end();
		rs.matrices.V = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
		rs.matrices.P = Mat4::identity();
		rs.matrices.PV = rs.matrices.P * rs.matrices.V;
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		renderThis(postProcRenderer, rs);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
	}

	// time it took to render the current frame
	ss.lastRenderDuration = Time::runtime() - ss.renderStart;

}

#endif // SCENE_H

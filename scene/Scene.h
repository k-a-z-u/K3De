#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "Camera.h"
#include "Lighting.h"
#include "Renderable.h"

#include "../world/Water.h"
#include "../world/Terrain.h"

#include "../3D/AABBRenderable.h"

#include "../world/Skybox.h"

#include "../mesh/MeshFactory.h"
#include "../shader/ShaderFactory.h"
#include "../textures/TextureFactory.h"
#include "../material/MaterialFactory.h"

#include "../2D/UI.h"
#include "../2D/SpriteFactory.h"

#include "FPS.h"
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

	Lighting lighting;



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
	MaterialFactory matFac;

	Vec2 clipY;

	FPS fps;
	RenderState rs;
	SceneState ss;

	Shader* overwriteShader;
	//Mat4 shadowPV;

public:

	Scene() : waterRenderer(nullptr), shadowRenderer(nullptr), postProcRenderer(nullptr), skybox(nullptr), matFac(this) {
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

	/** maximal number of lights? */
	int getMaxLights() const {
		return MAX_LIGHTS;
	}



//	/** set the number of lights to use */
//	void setNumLights(const int num) {
//		lights.num = num;
//	}

//	/** get the number of active lights to use */
//	int getNumLights() const {
//		return lights.num;
//	}

	/** get the light behind the given index */
	Light& getLight(const int idx) {
		_assertTrue(idx < MAX_LIGHTS, "light index out of bounds");
		return lighting.getLight(idx);
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
		return fps.get();
	}

	/** get the scene's mesh-factory */
	MeshFactory& getMeshFactory() {return meshFac;}

	/** get the scene's texture-factory */
	TextureFactory& getTextureFactory() {return textureFac;}

	/** get the scene's shader-factory */
	ShaderFactory& getShaderFactory() {return shaderFac;}

	/** get the scene's sprite-factory */
	SpriteFactory& getSpriteFactory() {return spriteFac;}

	/** get the scene's material-factory */
	MaterialFactory& getMaterialFactory() {return matFac;}

	/** get the scene's camera */
	Camera& getCamera() {return cam;}

	/** get the scene's 2D ui */
	UI& getUI() {return ui;}

	void setOverwriteShader(Shader* shader) {
		this->overwriteShader = shader;
	}

	void setShadowPV(const Mat4& mat, const int lightIdx) {

		// this bias matrix is used to convert from [-1;+1] to [0;+1]
		// adding this directly to the matrix saves time within the shaders!
		static const float bias[16] = {
		    0.5, 0.0, 0.0, 0.5,
		    0.0, 0.5, 0.0, 0.5,
		    0.0, 0.0, 0.5, 0.5,
		    0.0, 0.0, 0.0, 1.0
		};
		static const Mat4 mBias = Mat4::fromRawRowMajor(bias);

		//this->shadowPV = mBias * mat;
		//lights.light[lightIdx].setShadowPV(mBias * mat);
		lighting.getLight(lightIdx).setShadowPV(mBias * mat);

	}

	/** get the scene's shadow renderer */
	ShadowRenderer* getShadowRenderer() {return shadowRenderer;}


	/** scene will be rendered soon */
	virtual void onBeforeRender() {;}

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
			if (r->castsShadows()) {
				renderThis(r, rs);
			}
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
//		if (s->hasUniform("PVshadow"))		{s->setMatrix("PVshadow", shadowPV);}
		if (s->hasUniform("time"))			{s->setFloat("time", ss.getCurrentTime().seconds());}

		// configure fixed parameters once [constant between frames]
		if (s->hasUniformBlock("Lights"))	{s->setVarOnce("Lights", lighting.getUBO());}
		if (s->hasUniform("screenWidth"))	{s->setFloatOnce("screenWidth", rs.screenWidht);}
		if (s->hasUniform("screenHeight"))	{s->setFloatOnce("screenHeight", rs.screenHeight);}

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



#endif // SCENE_H

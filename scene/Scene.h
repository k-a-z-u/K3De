#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "Camera.h"
#include "Lighting.h"
#include "IRenderable.h"



#include "../3D/AABBRenderable.h"



#include "../mesh/MeshFactory.h"
#include "../shader/ShaderFactory.h"
#include "../textures/TextureFactory.h"
#include "../material3/MaterialFactory3.h"

#include "../2D/UI.h"
#include "../2D/SpriteFactory.h"

#include "../InputListener.h"

#include "FPS.h"
#include "RenderState.h"
#include "SceneState.h"
#include "IAnimation.h"

class Water;
class Terrain;
class SkyBox;

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
class Scene : public InputListener {

protected:

	Lighting lighting;

	/** the window's and viewport's size. updated by the engine when the window is changed */
	ScreenSize size;

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

	std::vector<IRenderable*> particles;

	/** "normal" renderables */
	std::vector<IRenderable*> renderables;

	/** all active animations */
	std::vector<IAnimation*> animations;

	/** the sky */
	SkyBox* skybox;

	/** 2D UI renderables */
	UI ui;


	MeshFactory meshFac;
	ShaderFactory shaderFac;
	TextureFactory textureFac;
	SpriteFactory spriteFac;
	MaterialFactory3 matFac;

	Vec2 clipZ;

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

	void setClipZ(const float min, const float max) {
		clipZ.x = min;
		clipZ.y = max;
	}

	/** maximal number of lights? */
	int getMaxLights() const {
		return MAX_LIGHTS;
	}

	/** get the light behind the given index */
	Light& getLight(const int idx) {
		_assertTrue(idx < MAX_LIGHTS, "light index out of bounds");
		return lighting.getLight(idx);
	}

	Ambient& getAmbient() {
		return lighting.getAmbient();
	}

	/** get all renderables within the level */
	const std::vector<IRenderable*>& getRenderables() const {
		return renderables;
	}

	/** get all renderables within the level */
	std::vector<IRenderable*>& getRenderables() {
		return renderables;
	}

	/** add the given renderable to the scene */
	inline void add(IRenderable* r);

	void addParticles(IRenderable* ps) {
		particles.push_back(ps);
	}

	void addAnimation(IAnimation* anim) {
		animations.push_back(anim);
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
	inline const ScreenSize& getScreenSize() const;

	/** get the current screen size */
	inline ScreenSize* getScreenSizePtr();


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
	MaterialFactory3& getMaterialFactory() {return matFac;}

	/** get the scene's camera */
	Camera& getCamera() {return cam;}

	/** get the scene's 2D ui */
	UI& getUI() {return ui;}

	void setOverwriteShader(Shader* shader) {
		this->overwriteShader = shader;
	}

	/** set the shadow-PV-matrix for the given light. add bias: switch from -1:+1 to 0:1 */
	void setShadowPV(const Mat4& mat, const int lightIdx, const bool addBias = true) {

		// this bias matrix is used to convert from [-1;+1] to [0;+1]
		// adding this directly to the matrix saves time within the shaders!
		static const float bias[16] = {
			0.5, 0.0, 0.0, 0.5,
			0.0, 0.5, 0.0, 0.5,
			0.0, 0.0, 0.5, 0.5,
			0.0, 0.0, 0.0, 1.0
		};
		static const Mat4 mBias = Mat4::fromRawRowMajor(bias);

		if (addBias) {
			lighting.getLight(lightIdx).setShadowPV(mBias * mat);
		} else {
			lighting.getLight(lightIdx).setShadowPV(mat);
		}

	}

	void setClearColor(const Vec4& color) {
		this->clearColor = color;
		glClearColor(color.r, color.g, color.b, color.a);
	}

	const Vec4& getClearColor() {return clearColor;}



	/** get the scene's shadow renderer */
	ShadowRenderer* getShadowRenderer() {return shadowRenderer;}

	/** get the scene's water renderer */
	WaterRenderer* getWaterRenderer() {return waterRenderer;}

	/** get the scene's post process renderer */
	PostProcessRenderer* getPostProcessRenderer() {return postProcRenderer;}



	/** scene is set as the currently active scene */
	virtual void onBecomesActive() = 0;

	/** scene will be rendered soon */
	virtual void onBeforeRender(const SceneState& ss) = 0;

	/** scene receives key events */
	virtual void onKeyEvent(const int key, const int scancode, const int action, const int mods) = 0;

	/** scene receives mouse events */
	virtual void onCursorPosition(double absX, double absY, double dx, double dy) = 0;



private:

	friend class Engine;
	friend class WaterRenderer;
	friend class PostProcessRenderer;
	friend class ShadowRenderer;
	friend class ShadowRendererSimple;

	/** render the complete scene. requires multiple passes */
	inline void render();

	/** render everything visible to water reflection and refraction */
	inline void renderForWater();

	/** called when the window is resized */
	inline void resize(const ScreenSize& size);

public:

	/** render everything casting a shadow */
	inline void renderForShadows();

private:

	/** render everything for the normal pass */
	inline void renderForNormal();

	inline void renderUI();

	/** render one object (if enabled, visible, ...) */
	inline void renderThis(IRenderable* r, RenderState& rs);

};



#endif // SCENE_H

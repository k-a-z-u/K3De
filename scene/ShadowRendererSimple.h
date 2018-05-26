#ifndef SHADOWRENDERERSIMPLE_H
#define SHADOWRENDERERSIMPLE_H

#include "../gl/Framebuffer.h"
#include "../gl/Renderbuffer.h"
#include "../math/Random.h"
#include "../textures/TextureDebug.h"
#include <sstream>

class Scene;

class ShadowRendererSimple : public ShadowRenderer {

private:

	friend class Scene;

	int texW = 0;
	int texH = 0;

	// one shadow texture per light
	Texture* texShadows[MAX_LIGHTS];

	Framebuffer fbShadows;
	Shader* sShadowGenDirectional;
	Shader* sShadowGenPoint;

	Scene* scene;

public:

	/** ctor */
	inline ShadowRendererSimple(Scene* scene);

	inline void update() override;

	/** get the shadow-map-texture for the given light */
	inline Texture* getShadowTexture(const int lightIdx) override {return texShadows[lightIdx];}

	/** update underlying data when the scene was resized */
	inline void resize(const int w, const int h) override;

};

#include "Scene.h"
#include "../Engine.h"

ShadowRendererSimple::ShadowRendererSimple(Scene* scene) {

	this->scene = scene;
	//texW = 512;
	//texH = 512;
	//texW = Engine::get()->getSettings().screen.width;
	//texH = Engine::get()->getSettings().screen.height;

	// ensure all textures are nullptrs
	std::fill(&texShadows[0], &texShadows[MAX_LIGHTS], nullptr);

	resize(1024, 1024);

	//fbShadows.attachTextureDepth(texShadows);

	sShadowGenDirectional = scene->getShaderFactory().createFromCode(
		#include "../material/inc/shadowMapVertexDirectional.glsl"
		,
		#include "../material/inc/shadowMapFragment.glsl"
	);

	sShadowGenPoint = scene->getShaderFactory().createFromCode(
		#include "../material/inc/shadowMapVertexPoint.glsl"
		,
		#include "../material/inc/shadowMapFragment.glsl"
	);

}

void ShadowRendererSimple::resize(const int w, const int h) {

	(void) w;
	(void) h;

	// new size. currently we use a fixed size, independent of the scene's size
	const int nTexW = 1024;
	const int nTexH = 1024;

	// not changed? -> skip
	if (nTexW == texW && nTexH == texH) {return;}

	// store new size
	texW = nTexW;
	texH = nTexH;

	// construct one 2D shadow-map texture per light
	// [unfortunately one 3D texture for all lights does NOT work for depth-texture!, color-texture would probably work..]
	for (int i = 0; i < MAX_LIGHTS; ++i) {

		// allocate or resize
		if (!texShadows[i]) {
			texShadows[i] = scene->getTextureFactory().createDepthTexture(texW, texH);
			texShadows[i]->setFilter(TextureFilter::NEAREST, TextureFilter::NEAREST);		// is this better than linear??
			texShadows[i]->setWrapping(TextureWrapping::CLAMP, TextureWrapping::CLAMP);
		} else {
			texShadows[i]->bind(0);
			texShadows[i]->resize(texW, texH);
		}

		// done
		texShadows[i]->unbind(0);

	}

}

void ShadowRendererSimple::update() {

	//glCullFace(GL_FRONT);
	//scene->setOverwriteShader(sShadowGen);

	Camera& camera = scene->getCamera();

	// store current camera
	//camera.push();

	// switch to shadow's resolution
	camera.setViewportSize(texW, texH);

	// enable framebuffer rendering
	fbShadows.bind();

	// only shadow, no color
	fbShadows.noColor();

	// process each potential light
	for (int i = 0; i < MAX_LIGHTS; ++i) {

		// get the light
		const Light& light = scene->getLight(i);

		// skip inactive lights
		if (!light.isActive()) {continue;}

		// skip non-shadow-casting lights
		if (!light.getCastsShadows()) {continue;}

		// remember the camera
		camera.push();

		// setup
		switch(light.getType()) {
		case LightType::POINT: {

			// hack to produce something that looks like a point-source-shadow
			scene->setOverwriteShader(sShadowGenPoint);

			const Vec3 lightShadowDir(0,0,-1);

			camera.setPosition(light.getPosition());
			camera.setLookAt(light.getPosition() + lightShadowDir);
			camera.setUp(0,1,0);	// (x/x plane, y = up)
			//camera.setFOV(40);
			//scene->setShadowPV(scene->getCamera().getPVMatrix(), i);

			//camera.setOrthographicParams(-5, +5, +5, -5, 0.001, 50);
			scene->setShadowPV(scene->getCamera().getVMatrix(), i, false);	// TEST no projection. view only. no bias (stay within -1:+1 instead of 0:1). see shader
			break;
		}
		case LightType::DIRECTIONAL: {

			// project the scene
			scene->setOverwriteShader(sShadowGenDirectional);

			// TODO: better way / more correct way??
			// the camera currently sees some region
			// we need to cover the same region from the perspective of the light (camera installed at light's position)
			float far = (camera.getPosition() - camera.getLookAt()).length() * 2.0;		// TODO
			far = std::max(far, 30.0f);
			const float size = (camera.getPosition() - camera.getLookAt()).length() * std::tan( camera.getFOV_rad() / 1.2 );
			const float w = size * 1.25;
			const float h = size * 1.25;
			//const Vec3 pos = camera.getLookAt() - Vec3(10, -30, 10);
			const Vec3 pos = camera.getLookAt() - (light.getDirection().normalized() * far);


			camera.setPosition(pos);
			camera.setOrthographicParams(-w, +w, +h, -h, 0.001, far * 1.25);
			//camera.setPosition(light.getPosition());
			//scene->getCamera().setFOV(65);//, 0.01, 200);
			scene->setShadowPV(scene->getCamera().getPVMatrix(), i);
			break;

		}
		default:
			throw Exception("not yet supported light-type for shadowing");
		}



		// attach corresponding texture
		fbShadows.attachTextureDepth(texShadows[i]);

		// render
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);
		scene->renderForShadows();

		// debug the rendered depth-texture
		if (1 == 1) {
			TextureDebug::show("shadowTexture" + std::to_string(i), texShadows[i], GL_DEPTH_COMPONENT);
		}

		// restore the camera
		camera.pop();

	}

	// disable framebuffer rendering
	fbShadows.unbind();

	// restore
	scene->setOverwriteShader(nullptr);
	//scene->getCamera().pop();

}

#endif // SHADOWRENDERERSIMPLE_H

#ifndef WATERRENDERER_H
#define WATERRENDERER_H


#include "../gl/Framebuffer.h"
#include "../gl/Renderbuffer.h"
#include "../textures/TextureDebug.h"

class Scene;

class WaterRenderer {

private:

	static constexpr const char* NAME = "WaterR";

	int texW = 0;
	int texH = 0;

	friend class Scene;

	// water reflection
	Framebuffer fbReflect;
	Renderbuffer rbReflect;
	Texture* texReflect = nullptr;

	// water refraction
	Framebuffer fbRefract;
	Renderbuffer rbRefract;
	Texture* texRefract = nullptr;

	// water's depth
	Framebuffer fbDepth;
	//Renderbuffer rbDepth;
	Texture* texDepth = nullptr;

	Scene* scene;

	Shader* sWaterDepth;


public:

	inline WaterRenderer(Scene* scene);

	Texture* getReflect() {return texReflect;}

	Texture* getRefract() {return texRefract;}

	Texture* getDepth() {return texDepth;}

	inline void update();

	/** called when the scene is resized */
	inline void resize(const int w, const int h);

};

#include "Scene.h"

WaterRenderer::WaterRenderer(Scene* scene) {

	this->scene = scene;

	resize(
		scene->getScreenSize().viewport.width,
		scene->getScreenSize().viewport.height
	);

	sWaterDepth = scene->getShaderFactory().createFromCode(
		#include "../material/inc/shadowMapVertexDirectional.glsl"
		,
		#include "../material/inc/shadowMapFragment.glsl"
	);

}

void WaterRenderer::resize(const int w, const int h) {

	Debug(NAME, "resizing reflect/refract to " + std::to_string(w) + " x " + std::to_string(h));

	// new size.
	const int nTexW = w; // /2;
	const int nTexH = h; //	/2;

	// not changed? -> skip
	if (nTexW == texW && nTexH == texH) {return;}

	// store new size
	texW = nTexW;
	texH = nTexH;

	// allocate reflection
	if (!texReflect) {
		texReflect = scene->getTextureFactory().createRenderTexture(texW, texH);
	} else {
		texReflect->bind(0);
		texReflect->resize(texW, texH);
	}

	texReflect->setFilter(TextureFilter::LINEAR, TextureFilter::LINEAR);
	texReflect->setWrapping(TextureWrapping::CLAMP, TextureWrapping::CLAMP);

	fbReflect.bind();
	fbReflect.attachTextureColor(0, texReflect);
	fbReflect.attachRenderbufferDepth(&rbReflect, texW, texH);
	fbReflect.setupColorTextures();
	fbReflect.unbind();
	texReflect->unbind(0);

	// allocate refraction
	if (!texRefract) {
		texRefract = scene->getTextureFactory().createRenderTexture(texW, texH);
	} else {
		texRefract->bind(0);
		texRefract->resize(texW, texH);
	}

	texRefract->setFilter(TextureFilter::LINEAR, TextureFilter::LINEAR);
	texRefract->setWrapping(TextureWrapping::CLAMP, TextureWrapping::CLAMP);

	fbRefract.bind();
	fbRefract.attachTextureColor(0, texRefract);
	fbRefract.attachRenderbufferDepth(&rbRefract, texW, texH);
	fbRefract.setupColorTextures();
	fbRefract.unbind();
	texRefract->unbind(0);

	// allocate depth
	if (!texDepth) {
		texDepth = scene->getTextureFactory().createDepthTexture(texW, texH);
		//texDepth = scene->getTextureFactory().createRenderTexture(texW, texH);
	} else {
		texDepth->bind(0);
		texDepth->resize(texW, texH);
	}

	texDepth->setFilter(TextureFilter::LINEAR, TextureFilter::LINEAR);
	texDepth->setWrapping(TextureWrapping::CLAMP, TextureWrapping::CLAMP);

	fbDepth.bind();

	fbDepth.attachTextureDepth(texDepth);

	//fbDepth.attachTextureColor(0, texDepth);
	//fbDepth.attachRenderbufferDepth(&rbDepth, texW, texH);
	//fbDepth.setupColorTextures();

	fbDepth.unbind();
	texDepth->unbind(0);

}

void WaterRenderer::update() {

	Camera& camera = scene->getCamera();


	camera.setViewportSize(texW, texH);
	const float delta = 0.02;	// used to prevent some artifacts near the water-boundaries

	// reflect
	camera.push();
		scene->getCamera().mirrorZ(0);
		scene->setClipZ(-delta, 9999);		// render everything below the water level (camera is mirrored!)
		fbReflect.bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		scene->renderForWater();
		fbReflect.unbind();
	camera.pop();

	// refract
	scene->setClipZ(-9999, +delta);	// render everything below the water level (camera is normal!)
	fbRefract.bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene->renderForWater();
	fbRefract.unbind();

	// depth
	// this one is true if water is within the z-plane
	camera.push();
		scene->setOverwriteShader(sWaterDepth);			// sufficient and faster
		glCullFace(GL_FRONT);							// why do we have to change the culling?!
		scene->setClipZ(-9999, +0.01);					// render everything below the water level (camera is above the water)
		const float size = 3;							// TODO: water's size
		const float far = 3;							// TODO: water's depth?
		const Vec3 camPos(0,0,0.02);					// camera slightly above the water	TODO: depends on water and camera position
		const Vec3 lookPos(0,0,0);						// looking downwards (into to water)
		camera.setPosition(camPos);
		camera.setLookAt(lookPos);
		camera.setUp(0, 1, 0);
		camera.setOrthographicParams(-size, +size, -size, +size, 0.01, far);
		fbDepth.bind();
		glClear(GL_DEPTH_BUFFER_BIT);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// debug with color
		scene->renderForWater();
		fbDepth.unbind();
		glCullFace(GL_BACK);
		scene->setOverwriteShader(nullptr);
	camera.pop();

	//TextureDebug::show("waterReflect", texReflect, GL_RGB);
	//TextureDebug::show("waterRefract", texRefract, GL_RGB);
	TextureDebug::show("waterDepthTexture", texDepth, GL_DEPTH_COMPONENT);

	// done
	//scene->setClipY(-0.0005, 9999);
	//scene->setClipY(-9999, +0.05);		// render everything below the water level (camera is normal!)
	scene->setClipZ(-9999, +9999);		// render everything


}

#endif // WATERRENDERER_H

#ifndef WATERRENDERER_H
#define WATERRENDERER_H


#include "../gl/Framebuffer.h"
#include "../gl/Renderbuffer.h"

class Scene;

class WaterRenderer {

private:

	const char* NAME = "WaterR";

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

	Scene* scene;

public:

	inline WaterRenderer(Scene* scene);

	Texture* getReflect() {return texReflect;}

	Texture* getRefract() {return texRefract;}

	inline void update();

	/** called when the scene is resized */
	inline void resize(const int w, const int h);

};

#include "Scene.h"

WaterRenderer::WaterRenderer(Scene* scene) {

	this->scene = scene;

	resize(
		Engine::get()->getSettings().screen.width,
		Engine::get()->getSettings().screen.height
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
	if (texReflect) {scene->getTextureFactory().destroy(texReflect);}
	texReflect = scene->getTextureFactory().createRenderTexture(texW, texH);
	texReflect->setFilter(TextureFilter::LINEAR, TextureFilter::LINEAR);
	texReflect->setWrapping(TextureWrapping::CLAMP, TextureWrapping::CLAMP);
	//texReflect->setAnisotropic(0);
	fbReflect.bind();
	fbReflect.attachTextureColor(0, texReflect);
	fbReflect.attachRenderbufferDepth(&rbReflect, texW, texH);
	fbReflect.setupColorTextures();
	fbReflect.unbind();

	// allocate refraction
	if (texRefract) {scene->getTextureFactory().destroy(texRefract);}
	texRefract = scene->getTextureFactory().createRenderTexture(texW, texH);
	texRefract->setFilter(TextureFilter::LINEAR, TextureFilter::LINEAR);
	texRefract->setWrapping(TextureWrapping::CLAMP, TextureWrapping::CLAMP);
	//texRefract->setAnisotropic(0);
	fbRefract.bind();
	fbRefract.attachTextureColor(0, texRefract);
	fbRefract.attachRenderbufferDepth(&rbRefract, texW, texH);
	fbRefract.setupColorTextures();
	fbRefract.unbind();

}

void WaterRenderer::update() {


	scene->getCamera().setScreenSize(texW, texH);

	// reflect
	scene->getCamera().mirrorY(0);
	//scene->getLight(0).mirrorY(0);
	scene->setClipY(-0.0005, 9999);		// render everything below the water level (camera is mirrored!)
	fbReflect.bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene->renderForWater();
	fbReflect.unbind();
	scene->getCamera().mirrorY(0);
	//scene->getLight(0).mirrorY(0);

	// refract
	scene->setClipY(-9999, +0.0005);		// render everything below the water level (camera is normal!)
	fbRefract.bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene->renderForWater();
	fbRefract.unbind();

	// done
	scene->setClipY(-0.0005, 9999);
	//scene->setClipY(-9999, +0.05);		// render everything below the water level (camera is normal!)

}

#endif // WATERRENDERER_H

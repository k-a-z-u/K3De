#ifndef WATERRENDERER_H
#define WATERRENDERER_H


#include "../gl/Framebuffer.h"
#include "../gl/Renderbuffer.h"

class Scene;

class WaterRenderer {

private:

	int texW;
	int texH;

	friend class Scene;

	// water reflection
	Framebuffer fbReflect;
	Renderbuffer rbReflect;
	Texture* texReflect;

	// water refraction
	Framebuffer fbRefract;
	Renderbuffer rbRefract;
	Texture* texRefract;

	Scene* scene;

public:

	WaterRenderer(Scene* scene);

	void update();

};

#include "Scene.h"

WaterRenderer::WaterRenderer(Scene* scene) {

	this->scene = scene;
	texW = Engine::get()->getSettings().screen.width / 2;
	texH = Engine::get()->getSettings().screen.height / 2;

	texReflect = scene->getTextureFactory().createRenderTexture(texW, texH);
	texReflect->setFilter(TextureFilter::LINEAR, TextureFilter::LINEAR);
	fbReflect.attachTextureColor(0, texReflect);
	fbReflect.attachRenderbufferDepth(&rbReflect, texW, texH);

	texRefract = scene->getTextureFactory().createRenderTexture(texW, texH);
	texRefract->setFilter(TextureFilter::LINEAR, TextureFilter::LINEAR);
	fbRefract.attachTextureColor(0, texRefract);
	fbRefract.attachRenderbufferDepth(&rbRefract, texW, texH);

}

void WaterRenderer::update() {


	scene->getCamera().setScreenSize(texW, texH);

	// reflect
	scene->getCamera().mirrorY(0);
	scene->getLight(0).mirrorY(0);
	scene->setClipY(-0.05, 9999);		// render everything below the water level (camera is mirrored!)
	fbReflect.enable();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene->renderForWater();
	//fbReflect.disable();
	scene->getCamera().mirrorY(0);
	scene->getLight(0).mirrorY(0);

	// refract
	scene->setClipY(-9999, +0.05);		// render everything below the water level (camera is normal!)
	fbRefract.enable();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene->renderForWater();
	fbRefract.disable();

	// done
	scene->setClipY(-0.05, 9999);

}

#endif // WATERRENDERER_H

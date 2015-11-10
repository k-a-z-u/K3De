#ifndef SHADOWRENDERERVSM_H
#define SHADOWRENDERERVSM_H

#include "../gl/Framebuffer.h"
#include "../gl/Renderbuffer.h"
#include "../fx/TextureFX.h"

class Scene;

class ShadowRendererVSM {

private:

	friend class Scene;

	int texW;
	int texH;

	Framebuffer fbShadows;
	Renderbuffer rbShadows;
	Texture* texShadows;
	Texture* texShadowsBlur;
	Shader* sShadowGen;

	Shader* sBlur;

	Scene* scene;
	TextureFX fx;

public:

	ShadowRendererVSM(Scene* scene);

	void update();

	void blur();

	/** get the rendered shadow texture */
	Texture* getShadowTexture() {return texShadows;}

};

#include "Scene.h"
#include "../Engine.h"

ShadowRendererVSM::ShadowRendererVSM(Scene* scene) {

	this->scene = scene;
	texW = Engine::get()->getSettings().screen.width;
	texH = Engine::get()->getSettings().screen.height;

	texShadows = scene->getTextureFactory().createRenderTexture(texW, texH, GL_RG32F);
	texShadows->setFilter(TextureFilter::LINEAR, TextureFilter::LINEAR);
	texShadows->setWrapping(TextureWrapping::CLAMP, TextureWrapping::CLAMP);

	texShadowsBlur = scene->getTextureFactory().createRenderTexture(texW, texH, GL_RG32F);
	texShadowsBlur->setFilter(TextureFilter::LINEAR, TextureFilter::LINEAR);
	texShadowsBlur->setWrapping(TextureWrapping::CLAMP, TextureWrapping::CLAMP);

	fbShadows.attachRenderbufferDepth(&rbShadows, texW, texH);
	fbShadows.attachTextureColor(0, texShadows);
	sShadowGen = scene->getShaderFactory().create("shaderShadowMapVertex.glsl", "shaderShadowMapFragment2.glsl");

	sBlur = scene->getShaderFactory().create("blurVertex.glsl", "blurFragment.glsl");
	fx.setShader(sBlur);

}


void ShadowRendererVSM::update() {

	const Vec4 oldClearColor = scene->getClearColor();
	scene->setClearColor( Vec4(1,1,1,1) );

	scene->getCamera().push();

	scene->getCamera().setScreenSize(texW, texH);
	glCullFace(GL_FRONT);
	scene->setOverwriteShader(sShadowGen);
	scene->getCamera().setPosition(scene->getLight(0).getPosition());
	scene->setShadowPV(scene->getCamera().getPVMatrix());

	fbShadows.enable();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene->renderForShadows();
	fbShadows.disable();

	scene->setOverwriteShader(nullptr);
	glCullFace(GL_BACK);

	blur();

	scene->getCamera().pop();
	texShadows->bind(7);
	scene->setClearColor(oldClearColor);

}

void ShadowRendererVSM::blur() {
	fx.process(texShadows, texShadowsBlur, texW, texH);
	fx.process(texShadowsBlur, texShadows, texW, texH);
}


#endif // SHADOWRENDERERVSM_H

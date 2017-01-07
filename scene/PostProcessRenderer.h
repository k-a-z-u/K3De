#ifndef POSTPROCESSRENDERER_H
#define POSTPROCESSRENDERER_H


#include "../gl/Framebuffer.h"
#include "../gl/Renderbuffer.h"
#include "../2D/Sprite.h"

class Scene;

class PostProcessRenderer : public Sprite2D {

private:

	friend class Scene;

	// usually the screen's pixel size
	int texW;
	int texH;

	// render to texture
	Framebuffer fb;
	Renderbuffer rb;
	Texture* tex = nullptr;

	// the pp-shader
//	Shader* shader = nullptr;

	Scene* scene = nullptr;


public:

	/** ctor */
	inline PostProcessRenderer(Scene* scene);

	inline void begin();

	inline void end();

	inline void render(const SceneState& ss, const RenderState& rs) override;


//	void setShader(Shader* shader) {
//		this->shader = shader;
//	}

};

#include "Scene.h"

PostProcessRenderer::PostProcessRenderer(Scene* scene) {

	this->scene = scene;
	texW = Engine::get()->getSettings().screen.width;
	texH = Engine::get()->getSettings().screen.height;

	tex = scene->getTextureFactory().createRenderTexture(texW, texH);
	tex->setFilter(TextureFilter::LINEAR, TextureFilter::LINEAR);
	fb.attachTextureColor(0, tex);
	fb.attachRenderbufferDepth(&rb, texW, texH);

	setRect(-1, -1, +1, +1);

}

void PostProcessRenderer::render(const SceneState& ss, const RenderState& rs) {
	tex->bind(0);
	Sprite2D::render(ss, rs);
	tex->unbind(0);
}

void PostProcessRenderer::begin() {
	fb.enable();
}

void PostProcessRenderer::end() {
	fb.disable();
	// TODO
}

#endif // POSTPROCESSRENDERER_H

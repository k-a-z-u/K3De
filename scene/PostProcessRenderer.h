#ifndef POSTPROCESSRENDERER_H
#define POSTPROCESSRENDERER_H


#include "../gl/Framebuffer.h"
#include "../gl/Renderbuffer.h"
#include "../2D/Sprite.h"

class Scene;
class PostProcessRenderer;
class RenderRect;

class RenderRect {

private:

	VAO vao;
	VBOArrayStatic<AttrVertexNormalTexture> vertices;

public:

	/** hidden CTOR */
	RenderRect() {
		vertices.resize(6);
	}

	void configure() {

		vao.bind();

		vertices.bind();
		vao.setVertices(0, 8*4);
		vao.setNormals(1, 8*4, 3*4);		// todo: remove
		vao.setTexCoords(2, 8*4, 6*4);

		vao.unbind();

	}

public:

	void setRect(const float x1, const float y1, const float x2, const float y2) {

		vertices.set(0, AttrVertexNormalTexture(x1,y1,0.1,		0,0,1,	0,0));
		vertices.set(1, AttrVertexNormalTexture(x2,y1,0.1,		0,0,1,	1,0));
		vertices.set(2, AttrVertexNormalTexture(x2,y2,0.1,		0,0,1,	1,1));

		vertices.set(3, AttrVertexNormalTexture(x1,y1,0.1,		0,0,1,	0,0));
		vertices.set(4, AttrVertexNormalTexture(x2,y2,0.1,		0,0,1,	1,1));
		vertices.set(5, AttrVertexNormalTexture(x1,y2,0.1,		0,0,1,	0,1));

		vertices.upload();
		configure();

	}

	void render() {
		vao.bind();
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		Error::assertOK();
		vao.unbind();
	}

};

class PostProcessStage {

	std::string name;
	Scene* scene;
	int w;
	int h;

	Multitexture texIn;

	Texture* texOut;

	Shader* shader = nullptr;

	std::vector<PostProcessStage*> previous;



public:

	PostProcessStage(const std::string& name, Scene* scene, const int w, const int h) : name(name), scene(scene), w(w), h(h) {
		texOut = scene->getTextureFactory().createRenderTexture(w, h);
		texOut->setWrapping(TextureWrapping::CLAMP, TextureWrapping::CLAMP);
		texOut->setFilter(TextureFilter::LINEAR, TextureFilter::LINEAR);
	}

	void setShader(Shader* s) {
		this->shader = s;
	}

	/** get this stage's output texture */
	Texture* getOutput() {
		return texOut;
	}

	/** attach a new input to this stage */
	void addInput(PostProcessStage* stage) {
		if (!this->shader) {throw Exception("set a shader before adding inputs");}
		previous.push_back(stage);
		addInput(stage->getOutput());
	}

protected:

	friend class PostProcessRenderer;

	void addInput(ITexture* tex) {
		const size_t idx = texIn.add(tex);
		const std::string name = "texColor" + std::to_string(idx);
		shader->bind();				// slow, but called only during the setup phase
		shader->setInt(name, idx);
		if (shader->hasUniform(name+"_w")) {shader->setFloat(name+"_w", tex->getWidth());}
		if (shader->hasUniform(name+"_h")) {shader->setFloat(name+"_h", tex->getHeight());}
		shader->unbind();
	}

	void buildChain(std::vector<PostProcessStage*>& chain) {
		for (PostProcessStage* s : previous) {
			if ("start" == s->name) {continue;}	// do NOT add the start element itself to the chain
			s->buildChain(chain);
			chain.push_back(s);
		}
	}



	void render(RenderRect& rr, const RenderState& rs) {

		scene->getCamera().push();
		scene->getCamera().setScreenSize(w, h);

		shader->bind();
//		const int w = (texIn) ? (texIn->getWidth()) : (rs.screenWidht);
//		const int h = (texIn) ? (texIn->getHeight()) : (rs.screenHeight);
//		if (shader->hasUniform("screenWidth"))	{shader->setFloat("screenWidth", w);}
//		if (shader->hasUniform("screenHeight"))	{shader->setFloat("screenHeight", h);}
		texIn.bindAll();
		rr.render();
		texIn.unbindAll();
		shader->unbind();

		scene->getCamera().pop();

	}

};

class PostProcessRenderer {

private:

	friend class Scene;

	// usually the screen's pixel size
	int texW;
	int texH;

	// render to texture
	Framebuffer fbStart;
	Texture* texDepth = nullptr;
	Renderbuffer rbDepth;
	Framebuffer fbChain;

	Scene* scene = nullptr;

	PostProcessStage* start = nullptr;
	PostProcessStage* out = nullptr;

	/** to be rendered fullscreen rectangle with texture coordinates */
	RenderRect rr;

	/** just for memory cleanup */
	std::vector<std::unique_ptr<PostProcessStage>> stages;


public:

	/** ctor */
	inline PostProcessRenderer(Scene* scene);

	/** get the output of the 3D stage, which is the input to all post-processing stages */
	PostProcessStage* getStart() {return start;}

	/** set the final element of the post-process stage which describes the whole chain in->out */
	void setOutput(PostProcessStage* out) {this->out = out;}

	/** build a new post-process stage */
	PostProcessStage* newStage(const std::string& name) {
		std::unique_ptr<PostProcessStage> uptr = std::make_unique<PostProcessStage>(name, scene, texW, texH);
		PostProcessStage* ptr = uptr.get();
		stages.push_back(std::move(uptr));
		return ptr;
	}

	/** build a new post-process stage */
	PostProcessStage* newStage(const std::string& name, const int texW, const int texH) {
		std::unique_ptr<PostProcessStage> uptr = std::make_unique<PostProcessStage>(name, scene, texW, texH);
		PostProcessStage* ptr = uptr.get();
		stages.push_back(std::move(uptr));
		return ptr;
	}


protected:

	friend class Scene;

	/** render the output of the final stage to the screen */
	inline void showResult(const SceneState& ss, const RenderState& rs);

	/** begin rendering 3D to a frambuffer texture */
	inline void begin3D();

	/** stop rendering 3D to a frambuffer texture */
	inline void end3D();

};

#include "Scene.h"

PostProcessRenderer::PostProcessRenderer(Scene* scene) {

	this->scene = scene;
	texW = Engine::get()->getSettings().screen.width;
	texH = Engine::get()->getSettings().screen.height;

	//texColor = scene->getTextureFactory().createRenderTexture(texW, texH);
	//texColor->setFilter(TextureFilter::LINEAR, TextureFilter::LINEAR);

	texDepth = scene->getTextureFactory().createDepthTexture(texW, texH);
	texDepth->setFilter(TextureFilter::LINEAR, TextureFilter::LINEAR);

	//fb.attachTextureColor(0, texColor);
	//fb.attachTextureDepth(texDepth);
	//fb.attachRenderbufferDepth(&rb, texW, texH);

	//setRect(-1, -1, +1, +1);
	rr.setRect(-1, -1, +1, +1);

	start = new PostProcessStage("start", scene, texW, texH);

	// framebuffer fbStart renders to texture
	fbStart.bind();
	fbStart.attachTextureColor(0, start->getOutput());
	//fbStart.attachTextureDepth(texDepth);
	fbStart.attachRenderbufferDepth(&rbDepth, texW, texH);		// slightly faster when depth is not needed
	fbStart.unbind();

}

void PostProcessRenderer::showResult(const SceneState& ss, const RenderState& rs) {

	// 3D scene was renderer into the start->getOutput();

	glDisable(GL_DEPTH_TEST);

	// build the post-processing chain
	std::vector<PostProcessStage*> chain;
	out->buildChain(chain);

	// enable the chain's framebuffer
	fbChain.bind();

	// render all inputs to the final postprocessing shader
	if (!chain.empty()) {

		// build all parts of the chain from input to output
		for (PostProcessStage* s : chain) {

			// ensure that this chain's link is rendered into a texture, attached to a framebuffer
			// this output-texture will be used as the input texture for the next link within the chain
			fbChain.attachTextureColor(0, s->getOutput());

			// render it
			s->render(rr, rs);

		}

	}

	// stop using the chain's framebuffer [= render to display]
	fbChain.unbind();

	// and render the final output [chain's last link] to screen [no framebuffer]
	out->render(rr, rs);

	glEnable(GL_DEPTH_TEST);

}

void PostProcessRenderer::begin3D() {

	// benchmarks indicated, this is the fastest solution:
	// use 2 framebuffers

	// render into the 3D-destination framebuffer
	fbStart.bind();

}

void PostProcessRenderer::end3D() {

	// stop rendering into the 3D-destination framebuffer
	fbStart.unbind();

}

#endif // POSTPROCESSRENDERER_H

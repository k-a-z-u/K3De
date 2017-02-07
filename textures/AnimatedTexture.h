#ifndef ANIMATEDTEXTURE_H
#define ANIMATEDTEXTURE_H

#include <vector>
#include "Texture.h"
#include "../Engine.h"

class AnimatedTexture : public ITexture {

private:

	float fps;
	std::vector<Texture*> textures;

public:

	AnimatedTexture(const float fps) : fps(fps) {
		;
	}

	void setFPS(const float fps) {
		this->fps = fps;
	}

	int getWidth() const override {
		return (textures.empty()) ? (-1) : (textures.front()->getWidth());
	}

	int getHeight() const override {
		return (textures.empty()) ? (-1) : (textures.front()->getHeight());
	}

	void add(Texture* tex) {
		textures.push_back(tex);
	}

	void bind(const TextureUnit idx) const override {
		get()->bind(idx);
	}

	void unbind(const TextureUnit idx) const override {
		get()->unbind(idx);
	}

	virtual bool isAlphaOnly() const override {
		return textures[0]->isAlphaOnly();
	}

	Texture* get() const {
		const int idx = int(fps * Time::runtime().seconds()) % textures.size();
		return textures[idx];
	}

};

#endif // ANIMATEDTEXTURE_H

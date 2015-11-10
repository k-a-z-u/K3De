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

	void add(Texture* tex) {
		textures.push_back(tex);
	}

	void bind(const int idx) const override {
		get()->bind(idx);
	}

	void unbind(const int idx) const override {
		get()->unbind(idx);
	}

	Texture* get() const {
		const float time = Engine::get()->getTimeSec();
		const int idx = (int) (fps * time) % textures.size();
		return textures[idx];
	}

};

#endif // ANIMATEDTEXTURE_H

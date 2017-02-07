#ifndef MULTITEXTURE_H
#define MULTITEXTURE_H

#include <vector>
#include "Texture.h"

/**
 * this is a helper class for working with multiple
 * texture-units/samplers on the GPU.
 *
 * textures can be assigned to each of the units/samplers using an index
 *
 * binding the multitexture will bind all contained textures,
 * each to its corresponding texture-unit/sampler on the GPU
 */
class Multitexture {

private:

	std::vector<ITexture*> textures;

public:

	/** set the texture behind the given sampler index */
	void set(const size_t idx, ITexture* texture) {
		if (textures.size() < (idx+1)) {textures.resize(idx+1);}
		textures[idx] = texture;
	}

	/** add to the next free slot. returns the used slot */
	size_t add(ITexture* tex) {
		const size_t idx = textures.size();
		textures.push_back(tex);
		return idx;
	}

	/** number of bound textures */
	size_t size() const {
		return textures.size();
	}

	/** bind all contained textures */
	void bindAll() {
		for (size_t i = 0; i < textures.size(); ++i) {
			if (textures[i]) {textures[i]->bind(i);}
		}
	}

	void unbindAll()  {
		for (size_t i = 0; i < textures.size(); ++i) {
			if (textures[i]) {textures[i]->unbind(i);}
		}
	}


	const std::vector<ITexture*> get() const {return textures;}

};

#endif // MULTITEXTURE_H

#ifndef SHADERSTATE_H
#define SHADERSTATE_H

struct ShaderState {

	/** index of the next usable texture slot */
	int nextFreeTextureSlot;

	ShaderState() : nextFreeTextureSlot(0) {;}

};

#endif // SHADERSTATE_H

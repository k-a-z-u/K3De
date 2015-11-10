#ifndef SHADOWRENDERER_H
#define SHADOWRENDERER_H

#include <string>
#include "../textures/Texture.h"

/**
 * interface for all shadow renderers
 */
class ShadowRenderer {

public:

	virtual ~ShadowRenderer() {;}

	/** update the shadow texture */
	virtual void update() = 0;

	/** get the rendered shadow texture */
	virtual Texture* getShadowTexture() = 0;

	/** glsl code to determine the amount of shadowing of the current fragment */
	virtual std::string getShadowAmountCalculationGLSL() = 0;

};

#endif // SHADOWRENDERER_H

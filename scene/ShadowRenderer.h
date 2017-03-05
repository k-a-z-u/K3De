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
	virtual Texture* getShadowTexture(const int lightIdx) = 0;

	/** called when the scene is resized */
	virtual void resize(const int w, const int h) = 0;

};

#endif // SHADOWRENDERER_H

#ifndef ITEXTURE_H
#define ITEXTURE_H

/** address one of the GPU's texture-units [0:...] */
using TextureUnit = unsigned int;

class ITexture {

protected:

public:

	virtual ~ITexture() {;}

	virtual void bind(const TextureUnit idx) const = 0;

	virtual void unbind(const TextureUnit idx) const = 0;

	/** texture's width */
	virtual int getWidth() const = 0;

	/** texture's height */
	virtual int getHeight() const = 0;

	/** is this an alpha-channel-only texture? */
	virtual bool isAlphaOnly() const = 0;


};

#endif // ITEXTURE_H

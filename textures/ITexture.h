#ifndef ITEXTURE_H
#define ITEXTURE_H

class ITexture {

protected:

public:

	virtual ~ITexture() {;}

	virtual void bind(const int idx) const = 0;

	virtual void unbind(const int idx) const = 0;


};

#endif // ITEXTURE_H

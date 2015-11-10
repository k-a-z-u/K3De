#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "../shader/Shader.h"
#include "../math/Math.h"
#include "RenderStage.h"
#include "../material/Material.h"

class Renderable {

protected:

	/** whether to render or skip this renderable */
	bool enabled;

	/** the shader to use for rendering this renderable */
	Shader* shader;

	/** the material to use for coloring this object */
	Material* material;

public:

	/** ctor */
	Renderable() : enabled(true), shader(nullptr), material(nullptr) {;}

	/** dtor */
	virtual ~Renderable() {;}

	Material* getMaterial() {return material;}
	void setMaterial(Material* mat) {this->material = mat;}

	/** render this object */
	virtual void render(const RenderStage& rs) = 0;

	/** get the object's model-matrix */
	virtual const Mat4& getMatrix() const = 0;

	/** is this renderable visible when applying the given MVP matrix? */
	virtual bool isVisible(const Mat4& MVP) const = 0;

	void setShader(Shader* shader) {this->shader = shader;}
	Shader* getShader() const {return shader;}

	void setEnabled(bool enabled) {this->enabled = enabled;}
	bool isEnabled() const {return enabled;}

};

#endif // RENDERABLE_H

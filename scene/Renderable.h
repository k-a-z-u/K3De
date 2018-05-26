#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "../material3/Material3.h"
#include "../material/Material2.h"

#include "../shader/Shader.h"
#include "../math/Math.h"

#include "IRenderable.h"
#include "RenderState.h"
#include "SceneState.h"

class Renderable : public IRenderable {

protected:

	/** whether to render or skip this renderable */
	bool enabled;

	/** the shader to use for rendering this renderable */
	Shader* shader;

	/** the material to use for coloring this object */
	Material3* material;

public:

	/** ctor */
	Renderable() : enabled(true), shader(nullptr), material(nullptr) {;}

	/** dtor */
	virtual ~Renderable() {;}


	/** does this renderable cast shadows? */
	bool castsShadows() const {
		return (material && material->getCastsShadows());
	}

//	/** this method is called, just before an object is rendered. once per complete frame */
//	virtual void onBeforeRender(const SceneState&) {;}

	/** render this object */
	virtual void render(const SceneState& ss, const RenderState& rs) = 0;


	/** get the object's model-matrix */
	virtual const Mat4& getMatrix() const = 0;

	/** is this renderable visible when applying the given MVP matrix? */
	virtual bool isVisible(const Mat4& MVP) const = 0;


	void setShader(Shader* shader) {this->shader = shader;}
	Shader* getShader() const {return shader;}

	void setEnabled(bool enabled) {this->enabled = enabled;}
	bool isEnabled() const {return enabled;}

	void setMaterial(Material3* mat) {
		Assert::isNull(this->material, "material already set.. would need some work to allow changes");

		if (!mat->getShader()) {
			throw Exception("compile the material before assigning it to a renderable");
		}
		this->material = mat;
		this->shader = mat->getShader();
	}

	void setMaterial(Material2* mat) {
		throw Exception("not supported");
	}

	Material3* getMaterial() const {
		return this->material;
	}

};

#endif // RENDERABLE_H

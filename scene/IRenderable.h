#ifndef IRENDERABLE_H
#define IRENDERABLE_H

class Material3;
class Shader;
class SceneState;
class RenderState;

/**
 * interface for all renderables that can be added to a scene
 */
class IRenderable {

public:

	/** does this renderable cast shadows? */
	virtual bool castsShadows() const = 0;

	/** is this element enabled, or should we skip it? */
	virtual bool isEnabled() const = 0;

	/** is this renderable visible when applying the given MVP matrix? */
	virtual bool isVisible(const Mat4& MVP) const = 0;


	/** get the object's model-matrix */
	virtual const Mat4& getMatrix() const = 0;

	virtual Material3* getMaterial() const = 0;

	/** get the shader to use for rendering */
	virtual Shader* getShader() const = 0;



	/** this method is called, just before an object is rendered. once per complete frame */
	virtual void onBeforeRender(const SceneState&) {;}

	/** render this object */
	virtual void render(const SceneState& ss, const RenderState& rs) = 0;

};

#endif // IRENDERABLE_H

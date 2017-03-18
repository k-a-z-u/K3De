#ifndef CAMERA_H
#define CAMERA_H

#include "../math/Math.h"
#include <list>
#include "../Engine.h"

/** all parameters defining the camera's state */
struct CameraState {

	struct {
		float fov = 45.0f;
		float near = 0.1f;		// MUST NOT BE 0!!!
		float far = 50.0f;
		float aspect = 4.0f/3.0f;	// 4.0f/3.0f
	} perspective;

	struct {
		float left;
		float right;
		float top;
		float bottom;
		float near;
		float far;
	} orthographic;

	bool usePerspective = true;

	Vec2 screenSize;

	Vec3 pos;
	Vec3 lookAt;
	Vec3 up;

};

class Camera {

private:

	bool stateChanged = true;

	CameraState state;

	std::list<CameraState> stack;

	Mat4 mP;
	Mat4 mV;
	Mat4 mPV;

public:

	/** store the current camera settings on the stack */
	void push() {
		stack.push_front(state);
	}

	/** restore the previously pushed settings from the stack */
	void pop() {
		state = stack.front();
		stateChanged = true;
		stack.pop_front();
		updateViewportSize();		// update
	}

	/** set the camera's field-of-view (in degree) */
	void setFOV(const float fov) {
		state.perspective.fov = fov;
		stateChanged = true;
	}

	/** whether to use perspective or orthographic projection */
	void setUsePerspective(const float perspective) {
		this->state.usePerspective = perspective;
		this->stateChanged = true;
	}

	/** set orhtographic projection params */
	void setOrthographicParams(const float left, const float right, const float top, const float bottom, const float near, const float far) {
		this->state.orthographic.left = left;
		this->state.orthographic.right = right;
		this->state.orthographic.top = top;
		this->state.orthographic.bottom = bottom;
		this->state.orthographic.near = near;
		this->state.orthographic.far = far;
		this->stateChanged = true;
	}

	/** set the render resolution (in pixel) */
	void setViewportSize(const int width, const int height) {
		state.screenSize.x = width;
		state.screenSize.y = height;
		setAspect((float)width/(float)height);
		updateViewportSize();
	}

	/** set the render resolution to the engine's screen size */
//	void setViewportSize(Scene* scene) {
//		setViewportSize(scene->getScreenSize().viewport.width, scene->getScreenSize().viewport.height);
//	}
    
//    /** set the render resolution */
//    void setViewportSize(const ScreenSize& size) {
//        setViewportSize(size.viewport.width, size.viewport.height);
//    }

	/** get the current screen size (resolution, in pixel) */
	const Vec2& getScreenSize() const {
		return state.screenSize;
	}

	/** set the aspect-ratio */
	void setAspect(const float aspect) {
		state.perspective.aspect = aspect;
		stateChanged = true;
	}

	const Vec3& getPosition() const {
		return state.pos;
	}

	const Vec3& getLookAt() const {
		return state.lookAt;
	}

	void setPosition(const float x, const float y, const float z) {
		state.pos.set(x,y,z);
		stateChanged = true;
	}

	void setPosition(const Vec3& pos) {
		state.pos = pos;
		stateChanged = true;
	}

	void setLookAt(const float x, const float y, const float z) {
		state.lookAt.set(x,y,z);
		stateChanged = true;
	}

	void setUp(const float x, const float y, const float z) {
		state.up.set(x,y,z);
		stateChanged = true;
	}

	void mirrorY(const float y) {
		(void) y; // TODO
		state.lookAt.y = -state.lookAt.y;
		state.pos.y = -state.pos.y;
		stateChanged = true;
	}

	const Mat4& getPVMatrix() {
		update();
		return mPV;
	}

	const Mat4& getVMatrix() {
		update();
		return mV;
	}

	const Mat4& getPMatrix() {
		update();
		return mP;
	}

private:

	/** update the matrices, if the state has changed */
	inline void update() {
		if (!stateChanged) {return;}
		stateChanged = false;
		mV = Math::camLookAt(state.pos, state.lookAt, state.up);
		if (state.usePerspective) {
			mP = Math::camPerspective(state.perspective.fov, state.perspective.aspect, state.perspective.near, state.perspective.far);
		} else {
			mP = Math::camOrthographic(state.orthographic.left, state.orthographic.right, state.orthographic.top, state.orthographic.bottom, state.orthographic.near, state.orthographic.far);
		}
		mPV = mP*mV;
	}

	/** update the openGL viewport size */
	void updateViewportSize() {
		glViewport(0, 0, state.screenSize.x, state.screenSize.y);
	}

};

#endif // CAMERA_H

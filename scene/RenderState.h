#ifndef RENDER_STATE_H
#define RENDER_STATE_H

#include "../math/Matrix.h"
#include "RenderPass.h"

struct RenderState {

	struct {
		Mat4 V;
		Mat4 P;
		Mat4 VM;
		Mat4 PV;
		Mat4 PVM;
	} matrices;

	RenderPass pass;

};

#endif // RENDER_STATE_H

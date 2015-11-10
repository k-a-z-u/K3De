#ifndef RENDERSTAGE_H
#define RENDERSTAGE_H

#include "../math/Matrix.h"

struct RenderStage {

	struct {
		Mat4 V;
		Mat4 P;
		Mat4 VM;
		Mat4 PV;
		Mat4 PVM;
	} matrices;

};

#endif // RENDERSTAGE_H

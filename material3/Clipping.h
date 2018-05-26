#ifndef PROV_CLIPPING_H
#define PROV_CLIPPING_H

#include "CodePart.h"

/** clipping within the fragment shader */
class ProvClipping {

public:

	virtual CodePart getFragment() const = 0;

};

/** clipping in the y-plane */
class ProvClippingY : public ProvClipping {

public:

	CodePart getFragment() const override {

		CodePart code;
		code.main += "if ((vertex_M.y < clipY[0]) || (vertex_M.y > clipY[1])) {discard;}";
		code.addUsage("vertex_M");
		code.addUsage("clipY");
		return code;

	}

};

/** clipping in the z-plane */
class ProvClippingZ : public ProvClipping {

public:

	CodePart getFragment() const override {

		CodePart code;
		code.main += "if ((vertex_M.z < clipZ[0]) || (vertex_M.z > clipZ[1])) {discard;}";
		code.addUsage("vertex_M");
		code.addUsage("clipZ");
		return code;

	}

};


#endif // PROV_CLIPPING_H

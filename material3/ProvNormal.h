#ifndef PROVNORMAL_H
#define PROVNORMAL_H

#include "CodePart.h"

/** provide the mesh's normal */
class ProvNormal {
public:
	virtual Var getVar() const = 0;
	virtual CodePart getFragment() const = 0;
	virtual CodePart getVertex() const = 0;
};


/** provide the mesh's default normal */
class ProvNormalDefault : public ProvNormal{

private:


public:

	Var getVar() const override {
		return VAR_FRAG_IN_VERTEX_NORMAL;
	}

	CodePart getFragment() const override {
		CodePart code;
		code.addUsage(FRAG_NORMAL_WORLD_POS);
		return code;
	}

	CodePart getVertex() const override {
		return CodePart();
	}

};

#endif // PROVNORMAL_H

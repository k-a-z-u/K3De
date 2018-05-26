#ifndef PROVCOLOR_H
#define PROVCOLOR_H

#include <string>
#include "CodePart.h"

class ProvColor {

protected:

	bool addedVertex = false;

	bool addedFragment = false;

	/** get the code to fill the variable */
	virtual CodePart getVertex() const = 0;

	/** get the code to fill the variable */
	virtual CodePart getFragment() const = 0;

public:

	/** get the variable that will contain the color */
	virtual Var getVar() const = 0;

	CodePart getVertexOnce() {
		if (!addedVertex) {
			addedVertex = true;
			return getVertex();
		} else {
			return CodePart();
		}
	}

	CodePart getFragmentOnce() {
		if (!addedFragment) {
			addedFragment = true;
			return getFragment();
		} else {
			return CodePart();
		}
	}

};

#endif // PROVCOLOR_H

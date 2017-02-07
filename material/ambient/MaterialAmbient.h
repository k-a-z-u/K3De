#ifndef MATERIALAMBIENT_H
#define MATERIALAMBIENT_H

#include "../MaterialElement.h"

/**
 * base-class for all ambient materials
 */
class MaterialAmbient : public MaterialElement {

public:

	/** dtor */
	virtual ~MaterialAmbient() {;}

};

#endif // MATERIALAMBIENT_H

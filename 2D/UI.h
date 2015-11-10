#ifndef UI_H
#define UI_H

#include <vector>
#include "../scene/Renderable.h"

class UI {

protected:

	friend class Scene;

	/** all elements within the UI */
	std::vector<Renderable*> elements;

public:

	/** add a new sprite to the UI */
	void add(Renderable* s) {
		elements.push_back(s);
	}

};

#endif // UI_H

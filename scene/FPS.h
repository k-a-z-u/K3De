#ifndef FPS_H
#define FPS_H

#include "../math/Time.h"

class Scene;

class FPS {

private:

	Time start;
	int frames = 0;
	float fps;

public:

	float get() const {
		return fps;
	}

protected:

	friend class Scene;

	void rendered() {

		++frames;

		if (frames > 100) {
			const Time cur = Time::runtime();
			fps = frames / (cur-start).seconds();
			start = cur;
			frames = 0;
		}

	}

};

#endif // FPS_H

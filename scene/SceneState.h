#ifndef SCENE_STATE_H
#define SCENE_STATE_H

#include "../math/Time.h"

struct SceneState {

private:

	friend class Scene;

	Time sceneStart;

	Time renderStart;
	Time lastRenderStart;
	Time lastRenderDuration;

public:

	/** get the current system time */
	Time getCurrentTime() const {
		return renderStart;
	}

	/** get the timestamp the last rendering was started at */
	Time getLastRenderStart() const {
		return lastRenderStart;
	}

	/** get the time it took to render the last frame */
	Time getLastRenderDuration() const {
		return lastRenderDuration;
	}

	/** get an fps indepentend measurement since the last frame */
	Time getTick() const {
		return renderStart - lastRenderStart;
	}

};

#endif // SCENE_STATE_H

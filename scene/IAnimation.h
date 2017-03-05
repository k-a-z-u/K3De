#ifndef IANIMATION_H
#define IANIMATION_H

#include "SceneState.h"

class Scene;

/**
 * interface for all animations that are stepped
 * before every newly rendered frame
 */
class IAnimation {

public:

	virtual ~IAnimation() {;}

	/** if an animation is done [returns true] it is deleted from the scene's animation list */
	virtual bool isDone() const = 0;

	/** process the animation's next step */
	virtual void step(Scene* scene, const SceneState& ss) = 0;

};

#endif // IANIMATION_H

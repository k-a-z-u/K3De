#ifndef INPUTLISTENER_H
#define INPUTLISTENER_H

class InputListener {

public:

	/** incoming keyboard event */
	virtual void onKeyEvent(const int key, const int scancode, const int action, const int mods) = 0;

};

#endif // INPUTLISTENER_H

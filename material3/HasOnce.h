#ifndef HASONCE_H
#define HASONCE_H

#include <string>

class HasOnce {

public:

	/** get code needed once within the shader */
	std::string getOnce() {
		static bool done = false;
		if (done) {return "";}
		done = true;
		return _getOnce();
	}

protected:

	/** code needed once within the shader */
	virtual std::string _getOnce() = 0;

};

#endif // HASONCE_H

#ifndef SCREENSIZE_H
#define SCREENSIZE_H

struct ScreenSize {

	int width;

	int height;

public:

	/** empty ctor */
	ScreenSize() : width(640), height(480) {
		;
	}

	/** ctor */
	ScreenSize(const int width, const int height) : width(width), height(height) {
		;
	}

	/** get width/height aspect ratio */
	float getAspect() const {
		return (float) width / (float) height;
	}

};

#endif // SCREENSIZE_H

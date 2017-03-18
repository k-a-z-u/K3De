#ifndef SCREENSIZE_H
#define SCREENSIZE_H

#include "../gl/gl.h"

struct ScreenSize {

    struct Window {
        int width;
        int height;
        Window() : width(0), height(0) {;}
        Window(const int width, const int height) : width(width), height(height) {;}
    } window;
    
    /** in case of retina, this might be twice the resolution above */
    struct Viewport {
        int width;
        int height;
        Viewport() : width(0), height(0) {;}
        Viewport(const int width, const int height) : width(width), height(height) {;}
    } viewport;

public:

	/** empty ctor */
	ScreenSize() : window(640, 480), viewport(640, 480) {
        ;
	}
    
    /** manual sizing ctor */
    ScreenSize(const int windowWidth, const int windowHeight) : window(windowWidth, windowHeight) {
        ;
    }

    /** ctor with the window to get the sizing for */
    ScreenSize(GLFWwindow* win) {
        update(win);
    }
    
    /** update window and viewport size for the given window. called when window size is changed */
    void update(GLFWwindow* win) {
        glfwGetWindowSize(win, &window.width, &window.height);
        glfwGetFramebufferSize(win, &viewport.width, &viewport.height);
        //viewport.width /= 2; // testing for retina
        //viewport.height /= 2;
    }
    
//	/** ctor */
//	ScreenSize(const int width, const int height) : Scene(width, height) {
//		GLFWwindow* window
//	}

	/** get width/height aspect ratio */
	float getAspect() const {
		return (float) window.width / (float) window.height;
	}

};

#endif // SCREENSIZE_H

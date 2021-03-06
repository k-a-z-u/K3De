#ifndef ENGINE_H
#define ENGINE_H

#include <iostream>

#include "EngineSettings.h"
#include "misc/Error.h"
#include "Debug.h"
#include "Exception.h"
#include "import/Image.h"


#include <KLib/Assertions.h>
#include <GLFW/glfw3.h>

#include "InputListener.h"

#include "threads/MainLoop.h"
#include "threads/GlobalThreadPool.h"

#define UNUSED(x)		(void) x

class Scene;

class Engine {

private:

	Scene* scene;
	EngineSettings settings;
	bool settingsOK = false;

	GLFWwindow* window;

	std::vector<InputListener*> inputListeners;

private:

	/** hidden ctor. use singleton */
	Engine() {

	}

public:

	/** grab current scene's viewport content */
	inline Image getFrame();

	/** singleton access */
	static Engine* get() {
		Engine* eng = singleton();
		if (!eng->settingsOK) {throw Exception("call Engine::init(..) first!");}
		return eng;
	}

	/** initialize the engine (once!) */
	static void init(const EngineSettings& settings) {
		Engine* eng = singleton();
		eng->apply(settings);
	}

	/** attach a new input listener */
	void addInputListener(InputListener* l) {
		this->inputListeners.push_back(l);
	}

	/** set the current scene */
	inline void setScene(Scene* scene);

	/** get the current scene */
	Scene* getScene() {
		return scene;
	}

	/** get the current screen size */
//	ScreenSize getScreenSize() const {
//		// TODO: change on window resize
//		return settings.screen;
//	}

	/** run blocking using the given framerate */
	inline void run(const int fps);

	/** perform cleanups */
	void cleanup() {
		if (window) {
			glfwDestroyWindow(window);
			window = nullptr;
		}
	}


	/** get the engine's configuration */
	const EngineSettings& getSettings() const {return settings;}

	/** one step further. render, other actions, etc. */
	inline void tick();

	/** get keyboard key state */
	int getKey(int key) {
		return glfwGetKey(window, key);
	}

	/** get keyboard key state */
	bool isKeyDown(int key) {
		return glfwGetKey(window, key) == GLFW_PRESS;
	}

	/** keep the mouse within the window */
	void captureMouse(bool capture) {
		if (capture) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		} else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}

private:

	/** called when the window cahnges its size */
	static inline void onWindowSizeChange(GLFWwindow* window, int width, int height);

	/** static key-input callback */
	static inline void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	/** static cursor-pos callback */
	static inline void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);


private:

	/** render the current scene */
	inline void render();

	/** hidden singleton access. public singleton access has additional checks */
	static Engine* singleton() {
		static Engine instance;
		return &instance;
	}

	/** configure the engine ONCE */
	void apply(EngineSettings settings) {

		// sanity check
		if (this->settingsOK) {throw Exception("must not initialize the engine twice!");}

		// init GLFW
		if( !glfwInit() ) {throw Exception("error during init");}
		Error::assertOK();

//		// this one is needed, when not OpenGL ES but OpenGL is used
//		switch(getOpenGLVersion()) {

//			case OPENGL_3_3:
//				glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
//				glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//				glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
//				glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL
//				Error::assertOK();
//				break;

//			default:
//				break;

//		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL
		Error::assertOK();

		// anti-aliasing?
		if (settings.antiAliasing) {
			glfwWindowHint(GLFW_SAMPLES, settings.antiAliasing);
			Error::assertOK();
		}

		// create a window
		window = glfwCreateWindow( settings.windowWidth, settings.windowHeight, settings.title.c_str(), NULL, NULL);
		if( window == NULL ) {glfwTerminate(); throw Exception("error while creating window");}
		Error::assertOK();

		// Display OpenGL version
		int major, minor, rev;
		glfwGetVersion(&major, &minor, &rev);
		std::cout << "OpenGL - " << major << "." << minor << "." << rev << std::endl;

		glfwMakeContextCurrent(window);
		Error::assertOK();

		#if defined (__APPLE__)
			// apple does not need GLEW!
		#else
			glewExperimental = true;
			if (glewInit() != GLEW_OK) {throw Exception("error during glewInit()");}
			glGetError();	// reset potential errors
			//Error::assertOK();
		#endif

		// Ensure we can capture the escape key being pressed below
		glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
		Error::assertOK();

		// capture resize event
		glfwSetWindowSizeCallback(window, Engine::onWindowSizeChange);

		// store
		this->settings = settings;
		this->settingsOK = true;

		// defaults
//		glEnable(GL_BLEND);
//		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// register for keyboard events
		glfwSetKeyCallback(window, keyCallback);
		glfwSetCursorPosCallback(window, cursorPosCallback);

	}


private:



//	void updateFPS() {

//		const float alpha = 0.95f;
//		this->fps = this->fps * alpha + getFPS() * (1.0f - alpha);

////		static uint64_t msA = getTimeMS();
////		static int numFrames = 0;
////		++numFrames;
////		const uint64_t diff = getTimeMS() - msA;
////		if (diff > 1000) {
////			const float fps = (float)numFrames / (float)diff * 1000.0f;
////			const float msPerFrame = (float)diff / (float)numFrames;
////			msA = getTimeMS();
////			numFrames = 0;
////			this->fps = fps;
////			//std::cout << "fps: " << fps << " msPerFrame: " << msPerFrame << std::endl;
////		}
//	}

};




#endif // ENGINE_H

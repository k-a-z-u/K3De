#ifndef ENGINE_H
#define ENGINE_H




#include <iostream>

#include "EngineSettings.h"
#include "misc/Error.h"
#include "Debug.h"
#include "Exception.h"
#include "import/Image.h"
//#include "scene/Scene.h"

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


	Image getFrame() {
		const int w = getScreenSize().width;
		const int h = getScreenSize().height;
		Image img(w, h, ImageFormat::IMAGE_RGB);
		//glReadBuffer(GL_BACK_LEFT);
		glReadPixels(0,0, w,h, GL_RGB,GL_UNSIGNED_BYTE, img.getDataPtr());
		return img;
	}



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
	void setScene(Scene* scene) {
		this->scene = scene;
	}

	/** get the current scene */
	Scene* getScene() {
		return scene;
	}

	/** get the current screen size */
	ScreenSize getScreenSize() const {
		// TODO: change on window resize
		return settings.screen;
	}

	//int i = 0;
	void run() {

		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		do {
			render();
			//if (++i > 2048) {break;}
		} while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0 );

	}

	/** render the current scene */
	inline void render();

	/** get the engine's configuration */
	const EngineSettings& getSettings() const {return settings;}

private:

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

//		// opengl version
//		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
//		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
//		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL
//		Error::assertOK();

//		// anti-aliasing?
//		if (settings.antiAliasing) {
//			glfwWindowHint(GLFW_SAMPLES, settings.antiAliasing);
//			Error::assertOK();
//		}

		// create a window
		window = glfwCreateWindow( settings.screen.width, settings.screen.height, settings.title.c_str(), NULL, NULL);
		if( window == NULL ) {glfwTerminate(); throw "error while creating window";}
		Error::assertOK();

		glfwMakeContextCurrent(window);
		glewExperimental = true;
		if (glewInit() != GLEW_OK) {throw "error during glewInit()";}
		Error::assertOK();

		// Ensure we can capture the escape key being pressed below
		glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
		Error::assertOK();

		// store
		this->settings = settings;
		this->settingsOK = true;

		// defaults
//		glEnable(GL_BLEND);
//		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// register for keyboard events
		glfwSetKeyCallback(window, keyCallback);

	}

private:

	/** static key-input callback */
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		for (InputListener* l : Engine::get()->inputListeners) {
			l->onKeyEvent(key, scancode, action, mods);
		}
	}

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

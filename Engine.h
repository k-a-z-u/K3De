#ifndef ENGINE_H
#define ENGINE_H



#include <chrono>
#include <iostream>

#include "EngineSettings.h"
#include "misc/Error.h"
#include "Debug.h"

#include <KLib/Assertions.h>
#include <GLFW/glfw3.h>


class Scene;

class Engine {

private:

	uint64_t msStart;
	uint64_t msCur;
	uint64_t msLast;
	uint64_t msSinceStart;
	Scene* scene;
	EngineSettings settings;

	GLFWwindow* window;

private:

	Engine() {
		msStart = _getMS();
		msLast = _getMS();
		msCur = _getMS();
	}

	uint64_t _getMS() {
		return std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
	}

public:

	void next() {
		msLast = msCur;
		msCur = _getMS();
		msSinceStart = msCur - msStart;
		updateFPS();
	}

	uint64_t getTimeMS() const {
		return msSinceStart;
	}

	float getTimeSec() const {
		return msSinceStart / 1000.0f;
	}

	float getDeltaSec() const {
		return (msCur - msLast) / 1000.0f;
	}

	/** singleton access */
	static Engine* get() {
		static Engine instance;
		return &instance;
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

	void apply(EngineSettings settings) {

		if( !glfwInit() ) {throw "error during init";}
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

		this->settings = settings;
		Error::assertOK();

	}

	//int i = 0;
	void run() {

		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		do {
			next();
			render();
			//if (++i > 2048) {break;}
		} while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0 );

	}

	void render();

	/** get the engine's configuration */
	const EngineSettings& getSettings() const {return settings;}

private:

	void updateFPS() {
		static uint64_t msA = getTimeMS();
		static int numFrames = 0;
		++numFrames;
		const uint64_t diff = getTimeMS() - msA;
		if (diff > 1000) {
			const float fps = (float)numFrames / (float)diff * 1000.0f;
			const float msPerFrame = (float)diff / (float)numFrames;
			msA = getTimeMS();
			numFrames = 0;
			std::cout << "fps: " << fps << " msPerFrame: " << msPerFrame << std::endl;
		}
	}

};


#include "scene/Scene.h"

void Engine::render() {

	_assertNotNull(window, "window is null. call Engine::apply(settings) first");

	scene->render();

	// make visible
	glfwSwapBuffers(window);
	glfwPollEvents();

}

#endif // ENGINE_H

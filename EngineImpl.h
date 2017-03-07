#ifndef ENGINEIMPL_H
#define ENGINEIMPL_H

#include "Engine.h"
#include "scene/Scene.h"
#include "threads/MainLoop.h"


void Engine::tick() {

	// process some elements from the main loop
	static const Time timeLimit = Time::fromMSec(100);
	MainLoop::get().execTime(timeLimit);

	// render the current scene
	render();

}

void Engine::onWindowSizeChange(GLFWwindow*, int width, int height) {

	auto action = [width, height] () {
		Engine::get()->settings.screen.width = width;
		Engine::get()->settings.screen.height = height;
		Scene* scene = Engine::get()->getScene();
		if (scene) {scene->resize(width, height);}
	};

	MainLoop::get().add(action);

}

void Engine::render() {

	_assertNotNull(window, "window is null. call Engine::apply(settings) first");
	_assertNotNull(scene, "scene is null. call setScene(..) first");

	// render the current scene
	scene->render();

	// make visible
	glfwSwapBuffers(window);
	glfwPollEvents();

}

void Engine::run(const int fps) {

	// convert fps to sleep timings
	const int sleep = 1000 / fps;
	int cnt = 0;

	// run until ESC is pressed or window's close symbol is clicked
	do {

		// render and analyzed the needed time
		const std::chrono::system_clock::time_point t1 = std::chrono::system_clock::now();
		tick();
		const std::chrono::system_clock::time_point t2 = std::chrono::system_clock::now();
		const int renderTime = (int) std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

		// sleep to approximate the requested FPS
		const int doSleep = sleep - renderTime;
		if (doSleep > 0) {
			std::this_thread::sleep_for(std::chrono::milliseconds(doSleep));
		}

		// debug output
        #if defined(WITH_DEBUG)
		   if (++cnt % 100 == 0) {
			   std::cout << getScene()->getFPS() << std::endl;
		   }
        #endif

	} while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0 );

	// cleanup
	cleanup();

}

/** set the current scene */
void Engine::setScene(Scene* scene) {
	this->scene = scene;
	scene->onBecomesActive();
}

/** static key-input callback */
void Engine::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	(void) window;
	for (InputListener* l : Engine::get()->inputListeners) {
		l->onKeyEvent(key, scancode, action, mods);
	}
	if (Engine::get()->getScene()) {
		Engine::get()->getScene()->onKeyEvent(key, scancode, action, mods);
	}
}



#endif // ENGINEIMPL_H

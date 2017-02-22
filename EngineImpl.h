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


void Engine::render() {

	_assertNotNull(window, "window is null. call Engine::apply(settings) first");
	_assertNotNull(scene, "scene is null. call setScene(..) first");

	// render the current scene
	scene->render();

	// make visible
	glfwSwapBuffers(window);
	glfwPollEvents();

}

/** set the current scene */
void Engine::setScene(Scene* scene) {
	this->scene = scene;
	scene->onBecomesActive();
}



#endif // ENGINEIMPL_H

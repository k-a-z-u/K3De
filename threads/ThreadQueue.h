#ifndef THREADQUEUE_H
#define THREADQUEUE_H

#include <vector>
#include <mutex>
#include <thread>
#include <functional>

#include "Semaphore.h"

#include "../Debug.h"

class ThreadQueue {

public:

	using Action = std::function<void()>;

private:

	std::string id;

	Semaphore sema;

	std::vector<Action> actions;
	std::mutex mtxActions;
	bool enabled = false;

	std::thread thread;

public:

	/** ctor */
	ThreadQueue(const std::string& id) : id(id) {
		;
	}

	/** add a new action to be executed in the background */
	void add(Action a) {
		Debug(id, "adding a new action");
		mtxActions.lock();
		actions.push_back(a);
		mtxActions.unlock();
		sema.notify();
	}

	/** number of currently "pending" tasks */
	size_t size() {
		mtxActions.lock();
		const size_t size = actions.size();
		mtxActions.unlock();
		return size;
	}

	/** start this queue */
	void start() {
		if (enabled) {return;}
		enabled = true;
		thread = std::thread(&ThreadQueue::loop, this);
		Debug(id, "started");
	}

	/** stop this queue */
	void stop() {
		enabled = false;
		Debug(id, "stopping");
	}

	/** wait for the thread to join */
	void join() {
		if (!enabled) {return;}
		thread.join();
	}

private:

	/** execute the next action */
	void execNext() {
		Action a;
		mtxActions.lock();
		a = actions.front();
		actions.erase(actions.begin());
		mtxActions.unlock();
		Debug(id, "executing next element");
		a();
	}

	/** background loop */
	void loop() {

		Debug(id, "running");

		while(enabled) {
			sema.wait();
			execNext();
		}

		Debug(id, "stopped");

	}

};

#endif // THREADQUEUE_H

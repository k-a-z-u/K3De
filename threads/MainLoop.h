#ifndef MAINLOOP_H
#define MAINLOOP_H



#include <vector>
#include <mutex>
#include <functional>

#include "../math/Time.h"
#include "../Debug.h"

class MainLoop {

public:

	using Action = std::function<void()>;

private:

	const char* NAME = "MainLoop";


	std::vector<Action> actions;
	std::mutex mtxActions;

private:

	/** ctor */
	MainLoop() {
		;
	}

public:

	/** singleton access */
	static MainLoop& get() {
		static MainLoop loop;
		return loop;
	}

	/** add a new action to be executed in the background */
	void add(Action a) {
		Debug(NAME, "adding a new action");
		mtxActions.lock();
		actions.push_back(a);
		mtxActions.unlock();
	}

	/** number of currently "pending" tasks */
	size_t size() {
		mtxActions.lock();
		const size_t size = actions.size();
		mtxActions.unlock();
		return size;
	}

	/** currently empty? [no actions pending] */
	bool empty() const {
		return actions.empty();
	}

	/** execute the next action */
	void execNext() {
		Action a;
		mtxActions.lock();
		a = actions.front();
		actions.erase(actions.begin());
		mtxActions.unlock();
		Debug(NAME, "executing next element");
		a();
	}

	/** exec (max) cnt elements */
	void execNum(const int cnt) {

	std::cout << "execute elements until several MSec have passed by.. much better?!" << std::endl;
		for (int i = 0; (i < cnt && !empty()); ++i) {
			execNext();
		}

	}

	/** execute elements from the main loop until there are no more elements, or the given duration is reached */
	void execTime(const Time duration) {

		// execute elements until this timestamp is reached
		const Time until = Time::runtime() + duration;

		// execute
		while(true) {

			// reached time limit?
			if (Time::runtime() > until) {break;}

			// nothing to execute?
			if (empty()) {break;}

			execNext();

		}

	}


};

#endif // MAINLOOP_H

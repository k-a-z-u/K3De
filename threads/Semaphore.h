#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <condition_variable>
#include <mutex>

class Semaphore {

private:

	std::mutex mutex;

	std::condition_variable condition;

	/** available count. start empty */
	unsigned long count = 0;

public:
	void notify() {
		std::unique_lock<decltype(mutex)> lock(mutex);
		++count;
		condition.notify_one();
	}

	void wait() {
		std::unique_lock<decltype(mutex)> lock(mutex);
		while(!count) // Handle spurious wake-ups.
			condition.wait(lock);
		--count;
	}

	bool try_wait() {
		std::unique_lock<decltype(mutex)> lock(mutex);
		if(count) {
			--count;
			return true;
		}
		return false;
	}

};

#endif // SEMAPHORE_H

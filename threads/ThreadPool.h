#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include "ThreadQueue.h"
#include <algorithm>

class ThreadPool {

private:

	std::vector<ThreadQueue*> queues;

public:

	/** ctor */
	ThreadPool(int size) {
		init(size);
	}

	/** dtor */
	~ThreadPool() {
		join();
		for (ThreadQueue* q : queues) {
			q->join();
			delete q;
		}
		queues.clear();
	}

	/** add a new to-be-executed action */
	void add(ThreadQueue::Action a) {

		auto comp = [] (ThreadQueue* a, ThreadQueue* b) {return a->size() < b->size();};
		auto it = std::min_element(queues.begin(), queues.end(), comp);

		ThreadQueue* tq = *it;
		tq->add(a);

	}

	/** stop all running threads */
	void stop() {
		for (ThreadQueue* q : queues) {q->stop();}
	}

	/** join all running threads */
	void join() {
		for (ThreadQueue* q : queues) {q->join();}
	}

private:

	/** build queues */
	void init(const int size) {
		for (int i = 0; i < size; ++i) {
			const std::string name = "ThreadQueue" + std::to_string(i);
			queues.push_back(new ThreadQueue(name));
		}
		for (int i = 0; i < size; ++i) {
			queues[i]->start();
		}
	}

};

#endif // THREADPOOL_H

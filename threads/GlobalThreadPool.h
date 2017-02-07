#ifndef GLOBALTHREADQUEUE_H
#define GLOBALTHREADQUEUE_H

#include "ThreadPool.h"

class GlobalThreadPool {

public:

	static ThreadPool& get() {
		static ThreadPool q(4);
		return q;
	}

};

#endif // GLOBALTHREADQUEUE_H

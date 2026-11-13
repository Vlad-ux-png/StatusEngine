#pragma once
#include <mutex>

class CriticalSection {
private:
	std::mutex mutex;

public:
	CriticalSection() {}

	void enter() {
		mutex.lock();
	}

	void leave() {
		mutex.unlock();
	}
};

#pragma once
#include <mutex>

class CriticalSection {
private:
	std::mutex mutex;
public:
	CriticalSection();

	void enter();

	void leave();
};

#include <mutex>
#include "CriticalSection.h"

CriticalSection::CriticalSection() {}

void CriticalSection::enter() {
	mutex.lock();
}

void CriticalSection::leave() {
	mutex.unlock();
}

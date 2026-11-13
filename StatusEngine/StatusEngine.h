#pragma once
#include <vector>
#include "CriticalSection.h"
#include "Event.h"

class Printer {
public:
	void print(StatusEvent eventData);
};

class StatusEngine {
private:
	CriticalSection lockManager;
	std::vector<Printer*> printers;

public:
	StatusEngine();

	void registerPrinter(Printer* printer);

	void unregisterPrinter(Printer* printer);

	void trigger(const StatusEvent& eventData);

	void monitorProcess(uint32_t processId);
};

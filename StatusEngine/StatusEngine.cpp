#include <iostream>
#include <cstdint>
#include "Event.h"
#include "StatusEngine.h"

int main() {
	StatusEngine engine;
	Printer printer;

	engine.registerPrinter(&printer);

	uint32_t pid = 0;
	std::cout << "Enter PID: ";
	std::cin >> pid;

	engine.monitorProcess(pid);

	engine.unregisterPrinter(&printer);

	return 0;
}

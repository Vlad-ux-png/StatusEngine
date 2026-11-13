#pragma once
#include <vector>
#include <windows.h>
#include "CriticalSection.h"

class Printer {
public:
	void print(StatusEvent eventData) {
		std::cout << "Process " << eventData.getProcessId() << " Exited with code: "
			<< eventData.getCode() << std::endl;
	}
};

class StatusEngine {
private:
	CriticalSection lockManager;
	std::vector<Printer*> printers;

public:
	StatusEngine() {}

	void registerPrinter(Printer* printer) {
		lockManager.enter();
		try {
			if (printer != nullptr) {
				printers.push_back(printer);
			}
		}
		catch (...) {
			lockManager.leave();
			throw;
		}
		lockManager.leave();
	}

	void unregisterPrinter(Printer* printer) {
		lockManager.enter();
		try {
			for (size_t i = 0; i < printers.size(); i++) {
				if (printers[i] == printer) {
					printers.erase(printers.begin() + i);
					break;
				}
			}
		}
		catch (...) {
			lockManager.leave();
			throw;
		}
		lockManager.leave();
	}

	void trigger(const StatusEvent& eventData) {
		lockManager.enter();
		for (size_t i = 0; i < printers.size(); i++) {
			if (printers[i] != nullptr) {
				printers[i]->print(eventData);
			}
		}
		lockManager.leave();
	}

	void monitorProcess(uint32_t processId) {
		HANDLE processHandle = OpenProcess(SYNCHRONIZE | PROCESS_QUERY_INFORMATION, FALSE, processId);
		if (processHandle == nullptr) {
			std::cout << "Failed to open the process with PID " << processId << std::endl;
			return;
		}

		WaitForSingleObject(processHandle, INFINITE);

		DWORD exitCode = 0;
		GetExitCodeProcess(processHandle, &exitCode);

		CloseHandle(processHandle);

		trigger(StatusEvent(exitCode, processId));
	}
};

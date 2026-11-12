#include <iostream>
#include <cstdint>
#include "StatusEngine.h"

class CriticalSection {
private:
	RTL_CRITICAL_SECTION lockObject;

public:
	CriticalSection() {
		RtlInitializeCriticalSection(&lockObject);
	}

	~CriticalSection() {
		RtlDeleteCriticalSection(&lockObject);
	}

	void enter() {
		RtlEnterCriticalSection(&lockObject);
	}

	void leave() {
		RtlLeaveCriticalSection(&lockObject);
	}
};

class StatusEvent {
private:
	int returnCode;
	uint32_t processId;

public:
	StatusEvent(int code, uint32_t pid) {
		returnCode = code;
		processId = pid;
	}

	int getCode() {
		return returnCode;
	}

	uint32_t getProcessId() {
		return processId;
	}
};

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
	Printer* targetPrinter;

public:
	StatusEngine() {
		targetPrinter = nullptr;
	}

	void registerPrinter(Printer* printer) {
		lockManager.enter();
		targetPrinter = printer;
		lockManager.leave();
	}

	void trigger(StatusEvent eventData) {
		lockManager.enter();
		if (targetPrinter != nullptr) {
			targetPrinter->print(eventData);
		}
		lockManager.leave();
	}

	void monitorAndExit(uint32_t pid) {
		OBJECT_ATTRIBUTES objAttr = { sizeof(OBJECT_ATTRIBUTES), 0, 0, 0, 0, 0 };
		CLIENT_ID clientId = { 
			(void*)(uintptr_t)pid,
			0 
		};

		void* processHandle = 0;
		NTSTATUS status = NtOpenProcess(&processHandle, 0x00100000L | 0x0400, &objAttr, &clientId);
		if (status != STATUS_SUCCESS || !processHandle) {
			return;
		}

		NtWaitForSingleObject(processHandle, 0, 0);

		PROCESS_BASIC_INFORMATION pbi;
		uint32_t returnLength = 0;
		NTSTATUS queryStatus = NtQueryInformationProcess(processHandle, 0, &pbi, sizeof(pbi), &returnLength);

		int exitCode = 0;
		if (queryStatus == STATUS_SUCCESS) {
			exitCode = (int)(uintptr_t)pbi.ExitStatus;
		}

		trigger(StatusEvent(exitCode, pid));
	}
};

int main() {
	StatusEngine engine;
	Printer printer;

	engine.registerPrinter(&printer);

	uint32_t pid = 0;
	std::cout << "Enter PID: ";
	std::cin >> pid;

	engine.monitorAndExit(pid);

	return 0;
}

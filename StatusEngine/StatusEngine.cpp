#include <iostream>
#include <vector>
#include <cstdint>
#include "Nt.h"

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

class Event {
private:
	int eventType;
public:
	Event(int eventType) {
		this->eventType = eventType;
	}
};

class StatusEvent : public Event {
private:
	static const int statusEventType = 0;

	int returnCode;
	uint32_t processId;

public:
	StatusEvent(int code, uint32_t pid): Event(statusEventType) {
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
	std::vector<Printer*> printers;

	static const uint32_t PROCESS_QUERY_INFORMATION = 0x0400;
	static const uint32_t SYNCHRONIZE = 0x00100000L;

public:
	StatusEngine() {}

	void registerPrinter(Printer* printer) {
		lockManager.enter();
		if (printer != nullptr) {
			printers.push_back(printer);
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

	void monitorProcess(uint32_t pid) {
		OBJECT_ATTRIBUTES objAttr = { sizeof(OBJECT_ATTRIBUTES), 0, 0, 0, 0, 0 };
		CLIENT_ID clientId = { 
			(void*)(uintptr_t)pid,
			0 
		};

		void* processHandle = 0;
		NTSTATUS status = NtOpenProcess(&processHandle, SYNCHRONIZE | PROCESS_QUERY_INFORMATION, &objAttr, &clientId);
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

		NtClose(processHandle);

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

	engine.monitorProcess(pid);

	return 0;
}

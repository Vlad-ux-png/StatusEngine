#pragma once

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
	StatusEvent(int returnCode, uint32_t processId) : Event(statusEventType) {
		this->returnCode = returnCode;
		this->processId = processId;
	}

	int getCode() {
		return returnCode;
	}

	uint32_t getProcessId() {
		return processId;
	}
};

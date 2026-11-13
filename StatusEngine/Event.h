#pragma once
#include <cstdint>

class Event {
private:
	int eventType;
public:
	Event(int eventType);
};

class StatusEvent : public Event {
private:
	static const int statusEventType = 0;

	int returnCode;
	uint32_t processId;

public:
	StatusEvent(int returnCode, uint32_t processId);

	int getCode();

	uint32_t getProcessId();
};

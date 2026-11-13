#include <cstdint>
#include "Event.h"

Event::Event(int eventType) {
	this->eventType = eventType;
}

StatusEvent::StatusEvent(int returnCode, uint32_t processId) : Event(statusEventType) {
	this->returnCode = returnCode;
	this->processId = processId;
}

int StatusEvent::getCode() {
	return returnCode;
}

uint32_t StatusEvent::getProcessId() {
	return processId;
}
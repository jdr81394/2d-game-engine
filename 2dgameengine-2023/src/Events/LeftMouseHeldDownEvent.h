#ifndef LEFTMOUSEHELDDOWNEVENT_H
#define LEFTMOUSEHELDDOWNEVENT_H

#include "../ECS/ECS.h"
#include "./OtherKeys.h"
#include "../EventBus/Event.h"



class LeftMouseHeldDownEvent : public Event {
private:
	int x;
	int y;
	OtherKeys otherKeys;
public:

	LeftMouseHeldDownEvent(int x, int y, OtherKeys otherKeys = {}) :x(x), y(y), otherKeys(otherKeys) {}
	int GetX() { return x; }
	int GetY() { return y; }
	OtherKeys GetOtherKeys() { return otherKeys; };
};


#endif
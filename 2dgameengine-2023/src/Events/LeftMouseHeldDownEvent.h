#ifndef LEFTMOUSEHELDDOWNEVENT_H
#define LEFTMOUSEHELDDOWNEVENT_H

#include "../ECS/ECS.h"
#include "../EventBus/Event.h"

class LeftMouseHeldDownEvent : public Event {
private:
	int x;
	int y;
public:

	LeftMouseHeldDownEvent(int x, int y) :x(x), y(y) {}
	int GetX() { return x; }
	int GetY() { return y; }
};


#endif
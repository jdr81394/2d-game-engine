#ifndef LEFTMOUSECLICKEDEVENT_H
#define LEFTMOUSECLICKEDEVENT_H

#include "../EventBus/Event.h"
#include "../ECS/ECS.h"
#include <SDL.h>


class LeftMouseClickedEvent : public Event {

private:
	int x;
	int y;

public:
	LeftMouseClickedEvent(int iX, int iY) : x(iX), y(iY) {}

	int GetX() { return x; }
	int GetY() { return y; }


};


#endif
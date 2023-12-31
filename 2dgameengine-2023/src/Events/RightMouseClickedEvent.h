#ifndef RIGHTMOUSECLICKEDEVENT_H
#define RIGHTMOUSECLICKEDEVENT_H

#include "../EventBus/Event.h"
#include "../ECS/ECS.h"
#include <SDL.h>

class RightMouseClickedEvent : public Event {

private:
	int x;
	int y;

public:
	RightMouseClickedEvent(int iX, int iY) : x(iX), y(iY) {};

	int GetX() { return x; }
	int GetY() { return y; }


};

#endif
#pragma once

#include "stdafx.h"
#include <iostream>
#include "../Utilities/utilities.h"
//#include "Sound.h"
#include "SceneObject.h"

#include <map>

enum EventsEnum {
	NullEvent, FirstEvent, Collision
};

class Events
{
public:
	Events();
	EventsEnum firstEvC, firstEvN, ev1, ev2;

	int idSound;
	//map<int, Sound*> soundMap;

	vector<pair<int, int> > objectCollision;
	int ver;

	void Collisionn(int);
	void playSounds(int);

};


#pragma once

#include "../Reality/Universe.h"

#include <queue>

class SpaceTime
{
public:
	SpaceTime();
	~SpaceTime();

private:
	std::queue<Object*> m_objects;

};


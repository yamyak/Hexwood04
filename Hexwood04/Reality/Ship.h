#pragma once

#include "Object.h"

class Ship : public Object
{
public:
	Ship();
	~Ship();

	bool Run(std::mutex& mutex, std::queue<Object*>& queue);
};


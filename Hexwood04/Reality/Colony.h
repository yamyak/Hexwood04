#pragma once

#include "Object.h"

class Planet;

class Colony : public Object
{
public:
	inline Colony() {};
	Colony(int id, Planet* planet);
	~Colony();

	bool Run();

private:
	Planet* m_planet;
};


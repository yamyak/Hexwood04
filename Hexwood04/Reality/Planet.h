#pragma once

#include "Object.h"
#include "../Constants.h"

#include <vector>

using namespace Constants;

struct Resource
{
	ResourceType m_type;
	float m_max;
	float m_current;
};

class Planet :	public Object
{
public:
	inline Planet() {};
	Planet(int id, PlanetType type, PlanetEnvironment env, std::vector<Resource>& resources);
	~Planet();

	bool Run();
	void SetOccupied(bool status);
	bool GetOccupied();

private:
	bool m_occupied;
	PlanetType m_type;
	PlanetEnvironment m_environment;
	std::vector<Resource> m_resources;
};


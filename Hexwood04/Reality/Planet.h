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
	Planet();
	Planet(int id, PlanetType type, PlanetEnvironment env, std::vector<Resource>& resources);
	~Planet();

	int GetId();

private:
	int m_id;
	PlanetType m_type;
	PlanetEnvironment m_environment;
	std::vector<Resource> m_resources;
};


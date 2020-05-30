#pragma once

#include "Object.h"

enum PlanetType
{
	TERRESTRIAL = 0,
	GAS_GIANT,
	TYPE_MAX
};

enum PlanetEnvironment
{
	NONE = 0,
	VARIABLE,
	DESERT,
	VOLCANIC,
	OCEAN,
	ICE,
	BARREN,
	ENVIRONMENT_MAX
};

class Planet :	public Object
{
public:
	Planet();
	Planet(int id, PlanetType type, PlanetEnvironment env);
	~Planet();

	int GetId();

private:
	int m_id;
	PlanetType m_type;
	PlanetEnvironment m_environment;
};


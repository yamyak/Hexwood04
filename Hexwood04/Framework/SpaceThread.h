#pragma once

#include "../Constants.h"
#include "../Reality/Planet.h"
#include "StarTable.h"

#include <random>

/// <summary>
/// Thread that subsection of universe is populated on 
/// </summary>
class SpaceThread
{
public:
	SpaceThread(int seed);
	~SpaceThread();

	bool CreateUniverse(StarTable& starDB, int start, int end);

private:
	// maximum number of planets allowed in system
	int m_max_planets;
	// bounds on size of gas giant planet
	float m_gas_giant_min_size;
	float m_gas_giant_max_size;
	// bounds on size of terrestrial planet
	float m_terrestrial_min_size;
	float m_terrestrial_max_size;
	// table of average resource values based on class of planet
	std::vector<std::vector<float>> m_planet_resources;
	// table of resource modifiers (%) based on environment of planet
	std::vector<std::vector<float>> m_resource_modifiers;
	// table of resource frame recharge rate based on type of planet
	std::vector<std::vector<float>> m_resource_recharge_rates;

	// random number distributions
	std::default_random_engine m_generator;
	std::uniform_int_distribution<int> m_uniform_dist;
	std::normal_distribution<float> m_resource_normal_dist;
	std::normal_distribution<float> m_recharge_normal_dist;

	Star* CreateSystem(StarData& data);
	Resource CreateResource(ResourceType type, PlanetType planet, PlanetEnvironment env);
};


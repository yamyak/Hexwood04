#pragma once

#include "../Constants.h"
#include "../Reality/Planet.h"
#include "StarTable.h"

#include <random>

class SpaceThread
{
public:
	SpaceThread(int seed);
	~SpaceThread();

	bool CreateUniverse(StarTable& starDB, int start, int end);

private:
	int m_max_planets;
	float m_gas_giant_min_size;
	float m_gas_giant_max_size;
	float m_terrestrial_min_size;
	float m_terrestrial_max_size;
	std::vector<std::vector<float>> m_planet_resources;
	std::vector<std::vector<float>> m_resource_modifiers;
	std::vector<std::vector<float>> m_resource_recharge_rates;

	std::default_random_engine m_generator;
	std::uniform_int_distribution<int> m_uniform_dist;
	std::normal_distribution<float> m_resource_normal_dist;
	std::normal_distribution<float> m_recharge_normal_dist;

	Star* CreateSystem(StarData& data);
	Resource CreateResource(ResourceType type, PlanetType planet, PlanetEnvironment env);
};


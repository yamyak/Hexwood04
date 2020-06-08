#pragma once

#include "StarTable.h"
#include "../Reality/Universe.h"

#include <random>

void Create(int thread_count, StarTable& stars, Universe& verse);

class SpaceEngine
{
public:
	SpaceEngine(int seed);
	~SpaceEngine();

	static void Create_Thread(int seed, Universe& verse, StarTable& starDB, int start, int end);
	bool CreateUniverse(Universe& verse, StarTable& starDB, int start, int end);

private:
	int m_max_planets;
	float m_gas_giant_min_size;
	float m_gas_giant_max_size;
	float m_terrestrial_min_size;
	float m_terrestrial_max_size;
	std::vector<std::vector<float>> m_planet_resources;
	std::vector<std::vector<float>> m_resource_modifiers;

	std::default_random_engine m_generator;
	std::uniform_int_distribution<int> m_uniform_dist;
	std::normal_distribution<float> m_normal_dist;

	Star CreateSystem(Universe& verse, StarData& data);
	Resource CreateResource(ResourceType type, PlanetType planet, PlanetEnvironment env);
};


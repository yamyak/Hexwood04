#pragma once

#include "StarTable.h"
#include "../Reality/Universe.h"
#include "../Reality/Star.h"
#include "../Reality/Planet.h"

#include <random>

class RealityEngine
{
public:
	RealityEngine(int seed);
	~RealityEngine();

	bool GenerateUniverse(Universe& verse, StarTable& starDB, int start, int end);

private:
	int m_max_planets;
	float m_gas_giant_min_size;
	float m_gas_giant_max_size;
	float m_terrestrial_min_size;
	float m_terrestrial_max_size;
	std::vector<std::vector<float>> m_planet_resources;
	std::vector<std::vector<float>> m_resource_modifiers;

	std::default_random_engine m_generator;
	std::normal_distribution<float> m_distribution;

	Star GenerateSystem(StarData& data);
	Resource GenerateResource(ResourceType type, PlanetType planet, PlanetEnvironment env);
};


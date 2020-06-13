#pragma once

#include "../Reality/Universe.h"

#include <random>

using namespace Constants;

class LifeEngine
{
public:
	LifeEngine(int seed);
	~LifeEngine();

	bool CreateEmpires(Universe& verse);

private:
	std::vector<std::vector<float>> m_resource_consumption;
	std::vector<std::vector<int>> m_period_length;
	std::vector<std::vector<int>> m_period_sigma;

	std::default_random_engine m_generator;
	std::uniform_int_distribution<int> m_uniform_dist;
	std::normal_distribution<float> m_empire_normal_dist;

	std::map<CivilizationPeriod, int> CalculatePeriodLengths();
	std::map<CivilizationPeriod, std::map<ResourceType, float>> CalculateConsumptionRates();
};


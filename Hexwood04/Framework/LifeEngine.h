#pragma once

#include "../Reality/Universe.h"

#include <random>

class LifeEngine
{
public:
	LifeEngine(int seed);
	~LifeEngine();

	bool CreateEmpires(Universe& verse);

private:
	std::default_random_engine m_generator;
	std::uniform_int_distribution<int> m_uniform_dist;
	std::normal_distribution<float> m_normal_dist;
};


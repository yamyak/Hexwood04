#pragma once

#include "Object.h"
#include "../Constants.h"

#include <vector>
#include <atomic>

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
	inline Planet() : m_system_id(0), m_occupied(false), m_type((PlanetType)0), m_environment((PlanetEnvironment)0) {};
	Planet(int id, PlanetType type, PlanetEnvironment env, std::vector<Resource>& resources);
	~Planet();

	bool Run(std::mutex& mutex, std::queue<Object*>& queue);
	bool SetOccupied();
	int GetSystemId();

private:
	static std::atomic<int> m_global_id;
	int m_system_id;
	bool m_occupied;
	PlanetType m_type;
	PlanetEnvironment m_environment;
	std::vector<Resource> m_resources;
};
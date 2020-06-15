#pragma once

#include "Object.h"
#include "../Constants.h"

#include <map>
#include <atomic>

using namespace Constants;

class Star;

struct Resource
{
	ResourceType m_type;
	float m_max;
	float m_current;
	float recharge_rate;
};

class Planet :	public Object
{
public:
	inline Planet() : m_system_id(0), m_occupied(false), m_type((PlanetType)0), m_environment((PlanetEnvironment)0), m_star(nullptr) {};
	Planet(int id, PlanetType type, PlanetEnvironment env, Star* star, std::map<ResourceType, Resource>& resources);
	~Planet();

	void Run(std::mutex& mutex, std::queue<Object*>& queue);
	bool SetOccupied();
	int GetSystemId();
	std::map<ResourceType, float> CollectResources(std::map<ResourceType, float> needs);

private:
	static std::atomic<int> m_global_id;
	int m_system_id;
	bool m_occupied;
	Star* m_star;
	PlanetType m_type;
	PlanetEnvironment m_environment;
	std::map<ResourceType, Resource> m_resources;
};
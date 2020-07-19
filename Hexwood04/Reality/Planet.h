#pragma once

#include "Object.h"
#include "../Constants.h"

#include <map>
#include <atomic>

using namespace Constants;

class Star;

/// <summary>
/// Resource structure
/// </summary>
struct Resource
{
	ResourceType m_type;
	// maximum value planet is capable of
	float m_max;
	// current value
	float m_current;
	// amount resource recharges every year
	float recharge_rate;
};

/// <summary>
/// Planet object
/// </summary>
class Planet :	public Object
{
public:
	inline Planet() : m_occupied(false), m_type((PlanetType)0), m_environment((PlanetEnvironment)0), m_star(nullptr), m_age(0) {};
	Planet(PlanetType type, PlanetEnvironment env, Star* star, std::map<ResourceType, Resource>& resources);
	~Planet();

	void Run(std::mutex& mutex, std::queue<Object*>& queue);
	bool SetOccupied();
	bool GetOccupied();
	std::map<ResourceType, float> CollectResources(std::map<ResourceType, float> needs);
	Star* GetStar();
	PlanetType GetPlanetType();

private:
	static std::atomic<int> m_global_id;
	// flag for whether planet is occupied
	bool m_occupied;
	int m_age;
	// parent star
	Star* m_star;
	PlanetType m_type;
	PlanetEnvironment m_environment;
	// resources associated with planet
	std::map<ResourceType, Resource> m_resources;
};
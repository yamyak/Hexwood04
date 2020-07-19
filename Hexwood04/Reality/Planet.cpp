#include "Planet.h"
#include "Universe.h"


std::atomic<int> Planet::m_global_id = 0;

/// <summary>
/// Constructor
/// </summary>
/// <param name="type">Planet type: terrestrial or gas giant</param>
/// <param name="env">Planet environment</param>
/// <param name="star">Reference to parent star</param>
/// <param name="resources">Resource map data structure</param>
/// <returns></returns>
Planet::Planet(PlanetType type, PlanetEnvironment env, Star* star, std::map<ResourceType, Resource>& resources) : 
m_type(type), m_environment(env), m_resources(resources), m_occupied(false), m_star(star), m_age(0)
{
	// set id that is unique amongst all objects of same type
	SetId(m_global_id++);
}

/// <summary>
/// Destructor
/// </summary>
/// <returns></returns>
Planet::~Planet()
{

}

/// <summary>
/// Planet processing method
/// Recharges planet resources
/// </summary>
/// <param name="mutex">Processing queue mutex</param>
/// <param name="queue">Processing queue</param>
void Planet::Run(std::mutex& mutex, std::queue<Object*>& queue)
{
	std::lock_guard<std::mutex> lock(m_object_mutex);

	// only process the planet once per time frame
	// process if planet age lags the universe age
	if (m_age != Universe::GetInstance()->GetAge())
	{
		m_age = Universe::GetInstance()->GetAge();

		// iterate through all resources
		for (auto& resource : m_resources)
		{
			// recharge the resource based on recharge rate
			float current = resource.second.m_current * (1 + resource.second.recharge_rate);
			// bound the resource by maximum allowed on planet
			resource.second.m_current = current > resource.second.m_max ? resource.second.m_max : current;
		}
	}
}

/// <summary>
/// Attempt to occupy the planet
/// </summary>
/// <returns>Whether occupation was successful</returns>
bool Planet::SetOccupied()
{
	std::lock_guard<std::mutex> lock(m_object_mutex);

	if (m_occupied)
	{
		// if planet already occupied, report unsuccessful
		return false;
	}
	else
	{
		// if planet not occupied, set as occupied and report
		m_occupied = true;
		return true;
	}
}

/// <summary>
/// Returns whether planet is occupied
/// </summary>
/// <returns>Whether planet is occupied</returns>
bool Planet::GetOccupied()
{
	return m_occupied;
}

/// <summary>
/// Collect resource from planet
/// </summary>
/// <param name="needs">Map of resource needs for each resource</param>
/// <returns>Map of resources collected for each resource</returns>
std::map<ResourceType, float> Planet::CollectResources(std::map<ResourceType, float> needs)
{
	std::lock_guard<std::mutex> lock(m_object_mutex);

	std::map<ResourceType, float> collected;
	
	for(auto& need : needs)
	{
		// if needed resource is available in quantity needed
		if (m_resources[need.first].m_current >= need.second)
		{
			// collect full amount and decrement from planet
			collected[need.first] = need.second;
			m_resources[need.first].m_current -= need.second;
		}
		// if needed resource is only partially available or not at all
		else
		{
			// collect what is available and decrement from planet
			collected[need.first] = m_resources[need.first].m_current;
			m_resources[need.first].m_current = 0;
		}
	}

	return collected;
}

/// <summary>
/// Returns parent star
/// </summary>
/// <returns>Star pointer</returns>
Star* Planet::GetStar()
{
	return m_star;
}

/// <summary>
/// Returns planet type: terrestrial or gas giant
/// </summary>
/// <returns>Planet type</returns>
PlanetType Planet::GetPlanetType()
{
	return m_type;
}
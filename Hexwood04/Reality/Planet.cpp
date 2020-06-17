#include "Planet.h"

std::atomic<int> Planet::m_global_id = 0;

Planet::Planet(int id, PlanetType type, PlanetEnvironment env, Star* star, std::map<ResourceType, Resource>& resources) : m_system_id(id), 
m_type(type), m_environment(env), m_resources(resources), m_occupied(false), m_star(star)
{
	SetId(m_global_id++);
}

Planet::~Planet()
{

}

void Planet::Run(std::mutex& mutex, std::queue<Object*>& queue)
{

}

bool Planet::SetOccupied()
{
	std::lock_guard<std::mutex> lock(m_object_mutex);
	//Lock();

	if (m_occupied)
	{
		return false;
	}
	else
	{
		m_occupied = true;
		return true;
	}

	//Unlock();
}

bool Planet::GetOccupied()
{
	return m_occupied;
}

int Planet::GetSystemId()
{
	return m_system_id;
}

std::map<ResourceType, float> Planet::CollectResources(std::map<ResourceType, float> needs)
{
	std::lock_guard<std::mutex> lock(m_object_mutex);
	//Lock();

	std::map<ResourceType, float> collected;
	
	std::map<ResourceType, float>::iterator it = needs.begin();
	while (it != needs.end())
	{
		if (m_resources[it->first].m_current >= it->second)
		{
			collected[it->first] = it->second;
			m_resources[it->first].m_current -= it->second;
		}
		else
		{
			collected[it->first] = m_resources[it->first].m_current;
			m_resources[it->first].m_current = 0;
		}
	}

	//Unlock();

	return collected;
}

Star* Planet::GetStar()
{
	return m_star;
}
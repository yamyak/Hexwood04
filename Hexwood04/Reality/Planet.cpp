#include "Planet.h"
#include "Universe.h"

std::atomic<int> Planet::m_global_id = 0;

Planet::Planet(int id, PlanetType type, PlanetEnvironment env, Star* star, std::map<ResourceType, Resource>& resources) : m_system_id(id), 
m_type(type), m_environment(env), m_resources(resources), m_occupied(false), m_star(star), m_age(0)
{
	SetId(m_global_id++);
}

Planet::~Planet()
{

}

void Planet::Run(std::mutex& mutex, std::queue<Object*>& queue)
{
	std::lock_guard<std::mutex> lock(m_object_mutex);

	if (m_age != Universe::GetInstance()->GetAge())
	{
		m_age++;

		for (auto& resource : m_resources)
		{
			float current = resource.second.m_current * (1 + resource.second.recharge_rate);
			resource.second.m_current = current > resource.second.m_max ? resource.second.m_max : current;
		}
	}
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
	
	for(auto& need : needs)
	{
		if (m_resources[need.first].m_current >= need.second)
		{
			collected[need.first] = need.second;
			m_resources[need.first].m_current -= need.second;
		}
		else
		{
			collected[need.first] = m_resources[need.first].m_current;
			m_resources[need.first].m_current = 0;
		}
	}

	//Unlock();

	return collected;
}

Star* Planet::GetStar()
{
	return m_star;
}
#include "Planet.h"

std::atomic<int> Planet::m_global_id = 0;

Planet::Planet(int id, PlanetType type, PlanetEnvironment env, std::vector<Resource>& resources) : m_system_id(id), m_type(type), m_environment(env), m_resources(resources), m_occupied(false)
{
	SetId(m_global_id++);
}

Planet::~Planet()
{

}

bool Planet::Run(std::mutex& mutex, std::queue<Object*>& queue)
{
	return true;
}

bool Planet::SetOccupied()
{
	Lock();

	if (m_occupied)
	{
		return false;
	}
	else
	{
		m_occupied = true;
		return true;
	}

	Unlock();
}

int Planet::GetSystemId()
{
	return m_system_id;
}
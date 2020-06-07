#include "Planet.h"


Planet::Planet(int id, PlanetType type, PlanetEnvironment env, std::vector<Resource>& resources) : m_type(type), m_environment(env), m_resources(resources), m_occupied(false)
{
	SetId(id);
}

Planet::~Planet()
{

}

bool Planet::Run()
{
	return true;
}

void Planet::SetOccupied(bool status)
{
	m_occupied = status;
}

bool Planet::GetOccupied()
{
	return m_occupied;
}
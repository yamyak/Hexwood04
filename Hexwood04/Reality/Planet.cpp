#include "Planet.h"


Planet::Planet()
{
	m_id = 0;
	m_type = (PlanetType)0;
	m_environment = (PlanetEnvironment)0;
}

Planet::Planet(int id, PlanetType type, PlanetEnvironment env, std::vector<Resource> & resources) : m_id(id), m_type(type), m_environment(env)
{
	m_resources = resources;
}

Planet::~Planet()
{

}

int Planet::GetId()
{
	return m_id;
}

bool Planet::Run()
{
	return true;
}
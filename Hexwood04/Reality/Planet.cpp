#include "Planet.h"

Planet::Planet()
{
	m_id = 0;
}

Planet::Planet(int id, PlanetType type, PlanetEnvironment env) : m_id(id), m_type(type), m_environment(env)
{

}

Planet::~Planet()
{

}

int Planet::GetId()
{
	return m_id;
}
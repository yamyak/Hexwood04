#include "Star.h"


Star::Star(int id, std::string& name, float x, float y, float z) : m_name(name), m_x(x), m_y(y), m_z(z)
{
	SetId(id);
}

Star::~Star()
{

}

void Star::AddPlanet(Planet& planet)
{
	m_planets[planet.GetId()] = planet;
}

bool Star::Run()
{
	return true;
}

int Star::GetSystemSize()
{
	return (int)m_planets.size();
}

Planet& Star::GetPlanet(int index)
{
	return m_planets[index];
}
#include "Star.h"


Star::Star()
{

}

Star::Star(int id, std::string& name, float x, float y, float z) : m_id(id), m_name(name), m_x(x), m_y(y), m_z(z)
{

}

Star::~Star()
{

}

int Star::GetId()
{
	return m_id;
}

void Star::AddPlanet(Planet& planet)
{
	m_planets[planet.GetId()] = planet;
}

bool Star::Run()
{
	return true;
}

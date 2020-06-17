#include "Ship.h"
#include "Star.h"
#include "Planet.h"

std::atomic<int> Ship::m_global_id = 0;

Ship::Ship(Planet* planet, float speed, float x, float y, float z, bool interstellar, Object* obj) : m_source(planet),
	m_speed(speed), m_x(x), m_y(y), m_z(z), m_interstellar(interstellar), 
	m_star_destination(nullptr), m_planet_destination(nullptr), m_empire_id(0)
{
	SetId(m_global_id++);

	if (interstellar)
	{
		m_star_destination = static_cast<Star*>(obj);
	}
	else
	{
		m_planet_destination = static_cast<Planet*>(obj);
	}
}

Ship::~Ship()
{

}

void Ship::Run(std::mutex& mutex, std::queue<Object*>& queue)
{

}

int Ship::GetEmpireId()
{
	return m_empire_id;
}

void Ship::SetEmpireId(int id)
{
	m_empire_id = id;
}
#include "Colony.h"

Colony::Colony(int id, Planet* planet) : m_planet(planet)
{
	SetId(id);
}

Colony::~Colony()
{

}

bool Colony::Run()
{
	return false;
}

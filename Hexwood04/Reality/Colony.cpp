#include "Colony.h"

std::atomic<int> Colony::m_global_id = 0;

Colony::Colony(Planet* planet) : m_empire_id(0), m_planet(planet)
{
	SetId(m_global_id++);
}

Colony::~Colony()
{

}

bool Colony::Run()
{
	return false;
}

int Colony::GetEmpireId()
{
	return m_empire_id;
}

void Colony::SetEmpireId(int id)
{
	m_empire_id = id;
}
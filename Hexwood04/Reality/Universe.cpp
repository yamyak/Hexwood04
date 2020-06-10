#include "Universe.h"


Universe::Universe()
{

}

Universe::~Universe()
{

}

void Universe::AddStar(Star* star)
{
	std::lock_guard<std::mutex> guard(m_star_mutex);

	m_stars[star->GetId()] = star;
}

void Universe::AddPlanet(Planet* planet)
{
	std::lock_guard<std::mutex> guard(m_planet_mutex);

	m_planets[planet->GetId()] = planet;
}

void Universe::AddEmpire(Empire* empire)
{
	m_empires[empire->GetId()] = empire;
}

void Universe::AddColony(Colony* colony)
{
	std::lock_guard<std::mutex> guard(m_colony_mutex);

	m_colonies[colony->GetId()] = colony;
}

bool Universe::Run()
{
	return true;
}

int Universe::GetSize()
{
	return (int)m_stars.size();
}

Star* Universe::GetStar(int key)
{
	return m_stars[key];
}
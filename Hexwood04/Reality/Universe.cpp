#include "Universe.h"


Universe::Universe()
{

}

Universe::~Universe()
{

}

Star* Universe::AddStar(Star& star)
{
	std::lock_guard<std::mutex> guard(m_star_mutex);

	m_stars[star.GetId()] = star;
	return &m_stars[star.GetId()];
}

Planet* Universe::AddPlanet(Planet& planet)
{
	std::lock_guard<std::mutex> guard(m_planet_mutex);

	m_planets[planet.GetId()] = planet;
	return &m_planets[planet.GetId()];
}

Empire* Universe::AddEmpire(Empire& empire)
{
	m_empires[empire.GetId()] = empire;
	return &m_empires[empire.GetId()];
}

Colony* Universe::AddColony(Colony& colony)
{
	std::lock_guard<std::mutex> guard(m_colony_mutex);

	m_colonies[colony.GetId()] = colony;
	return &m_colonies[colony.GetId()];
}

bool Universe::Run()
{
	return true;
}

int Universe::GetSize()
{
	return (int)m_stars.size();
}

Star& Universe::GetStar(int key)
{
	return m_stars[key];
}
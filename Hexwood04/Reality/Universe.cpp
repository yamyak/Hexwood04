#include "Universe.h"
#include "Star.h"
#include "Empire.h"
#include "Planet.h"
#include "Colony.h"


Universe::Universe()
{

}

Universe::~Universe()
{

}

void Universe::AddStar(Star* star)
{
	std::lock_guard<std::mutex> lock(m_object_mutex);
	//std::lock_guard<std::mutex> lock(m_star_mutex);

	m_stars[star->GetId()] = star;
}

void Universe::AddPlanet(Planet* planet)
{
	std::lock_guard<std::mutex> lock(m_object_mutex);
	//std::lock_guard<std::mutex> lock(m_planet_mutex);

	m_planets[planet->GetId()] = planet;
}

void Universe::AddEmpire(Empire* empire)
{
	std::lock_guard<std::mutex> lock(m_object_mutex);
	//std::lock_guard<std::mutex> lock(m_empire_mutex);

	m_empires[empire->GetId()] = empire;
}

void Universe::AddColony(Colony* colony)
{
	std::lock_guard<std::mutex> lock(m_object_mutex);
	//std::lock_guard<std::mutex> lock(m_colony_mutex);

	m_colonies[colony->GetId()] = colony;
}

void Universe::Run(std::mutex& mutex, std::queue<Object*>& queue)
{
	std::lock_guard<std::mutex> lock(m_object_mutex);
	//Lock();

	//std::unique_lock<std::mutex> empire_lock(m_empire_mutex);
	for (auto& empire : m_empires)
	{
		std::lock_guard<std::mutex> queue_lock(mutex);
		queue.push(static_cast<Object*>(empire.second));
	}
	//empire_lock.unlock();

	//Unlock();
}

int Universe::GetSize()
{
	return (int)m_stars.size();
}

Star* Universe::GetStar(int key)
{
	return m_stars[key];
}
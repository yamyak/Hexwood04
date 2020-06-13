#pragma once

#include "Object.h"
#include "Star.h"
#include "Empire.h"

#include <mutex>

class Universe : public Object
{
public:
	Universe();
	~Universe();

	void AddStar(Star* star);
	void AddPlanet(Planet* planet);
	void AddEmpire(Empire* empire);
	void AddColony(Colony* colony);
	Star* GetStar(int key);
	int GetSize();
	void Run(std::mutex& mutex, std::queue<Object*>& queue);

private:
	std::map<int, Star*> m_stars;
	std::mutex m_star_mutex;
	std::map<int, Planet*> m_planets;
	std::mutex m_planet_mutex;

	std::map<int, Empire*> m_empires;
	std::mutex m_empire_mutex;
	std::map<int, Colony*> m_colonies;
	std::mutex m_colony_mutex;
};


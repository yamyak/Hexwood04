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

	Star* AddStar(Star& star);
	Planet* AddPlanet(Planet& planet);
	Empire* AddEmpire(Empire& empire);
	Colony* AddColony(Colony& colony);
	Star& GetStar(int key);
	int GetSize();
	bool Run();

private:
	std::map<int, Star> m_stars;
	std::mutex m_star_mutex;
	std::map<int, Planet> m_planets;
	std::mutex m_planet_mutex;

	std::map<int, Empire> m_empires;
	std::map<int, Colony> m_colonies;
	std::mutex m_colony_mutex;
};


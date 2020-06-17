#pragma once

#include "../Constants.h"

#include <mutex>
#include <map>
#include <vector>

using namespace Constants;

class Object;
class Empire;
class Colony;
class Star;
class Planet;
class Ship;

class Universe
{
public:
	Universe();

	static Universe* GetInstance();

	std::vector<Object*> GetObjects(ObjectType type);
	void AddStar(Star* star);
	void AddPlanet(Planet* planet);
	void AddEmpire(Empire* empire);
	void AddColony(Colony* colony);
	void AddShip(Ship* ship);
	Star* GetStar(int key);
	int GetSize();

private:
	static Universe* m_instance;

	std::mutex m_object_mutex;

	std::map<int, Star*> m_stars;
	std::mutex m_star_mutex;
	std::map<int, Planet*> m_planets;
	std::mutex m_planet_mutex;

	std::map<int, Empire*> m_empires;
	std::mutex m_empire_mutex;
	std::map<int, Colony*> m_colonies;
	std::mutex m_colony_mutex;
	std::map<int, Ship*> m_ships;
	std::mutex m_ship_mutex;

	~Universe();

	void CleanUp();
};


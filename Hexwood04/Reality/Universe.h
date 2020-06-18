#pragma once

#include "../Constants.h"

#include <mutex>
#include <map>
#include <vector>
#include <utility>

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
	Star* GetStar(int key);
	void AddPlanet(Planet* planet);
	void AddEmpire(Empire* empire);
	void AddColony(Colony* colony);
	void AddShip(Ship* ship);
	void AddToGraveyard(ObjectType type, int id);
	void ClearOutGraveyard();
	int GetSize();
	void IncrementAge();
	int GetAge();

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

	int m_age;
	std::vector<std::pair<ObjectType, int>> m_graveyard;

	~Universe();

	void CleanUp();
};


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
	int GetStarCount();
	void AddPlanet(Planet* planet);
	void AddEmpire(Empire* empire);
	int GetEmpireCount();
	void AddColony(Colony* colony);
	int GetColonyCount();
	void AddShip(Ship* ship);
	int GetShipCount();
	void AddToGraveyard(ObjectType type, int id);
	void ClearOutGraveyard();
	void IncrementAge();
	int GetAge();

private:
	static Universe* m_instance;

	std::mutex m_object_mutex;

	std::map<int, Star*> m_stars;
	std::map<int, Planet*> m_planets;

	std::map<int, Empire*> m_empires;
	std::map<int, Colony*> m_colonies;
	std::map<int, Ship*> m_ships;

	int m_age;
	std::vector<std::pair<ObjectType, int>> m_graveyard;

	~Universe();

	void CleanUp();
};


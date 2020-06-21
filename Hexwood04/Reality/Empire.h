#pragma once

#include "Object.h"
#include "../Constants.h"

#include <map>
#include <vector>

class Colony;
class Ship;

using namespace Constants;

class Empire : public Object
{
public:
	inline Empire() {};
	Empire(int id);
	~Empire();

	void Run(std::mutex& mutex, std::queue<Object*>& queue);
	void AddColony(Colony* colony);
	void AddShip(Ship* ship);
	void DeleteShip(int id);
	std::vector<Colony*> GetColonies();
	bool CheckOccupancy(ObjectType type, int id);
	void RegisterForOccupancy(ObjectType type, int id);
	void CleanOccupanyRecords(ObjectType type, int id);

private:
	std::map<int, Colony*> m_colonies;
	std::map<int, Ship*> m_ships;
	std::vector<int> m_potentialEmpireSystems;
	std::vector<int> m_potenialColonySites;
};


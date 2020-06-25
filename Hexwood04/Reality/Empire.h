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
	std::vector<Ship*> GetShips();
	bool CheckColonization(ObjectType type, int id);
	bool SetColonized(ObjectType type, int id, int colony_id);

	friend class Logger;

private:
	std::map<int, Colony*> m_colonies;
	std::map<int, Ship*> m_ships;
	std::map<int, int> m_potentialEmpireSystems;
	std::map<int, int> m_potenialColonySites;
};


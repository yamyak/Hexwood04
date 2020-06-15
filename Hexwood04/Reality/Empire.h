#pragma once

#include "Object.h"

#include <map>

class Colony;
class Ship;

class Empire : public Object
{
public:
	inline Empire() {};
	Empire(int id);
	~Empire();

	void Run(std::mutex& mutex, std::queue<Object*>& queue);
	void AddColony(Colony* colony);
	void AddShip(Ship* ship);
	std::map<int, Colony*>& GetColonies();

private:
	std::map<int, Colony*> m_colonies;
	std::map<int, Ship*> m_ships;
};


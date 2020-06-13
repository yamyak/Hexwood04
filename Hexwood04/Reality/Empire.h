#pragma once

#include "Object.h"
#include "Colony.h"

#include <map>

class Empire : public Object
{
public:
	inline Empire() {};
	Empire(int id);
	~Empire();

	void Run(std::mutex& mutex, std::queue<Object*>& queue);
	void AddColony(Colony* colony);
	std::map<int, Colony*>& GetColonies();

private:
	std::map<int, Colony*> m_colonies;
};


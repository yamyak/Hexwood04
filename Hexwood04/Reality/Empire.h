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

	bool Run();
	void AddColony(Colony& colony);

private:
	std::map<int, Colony> m_colonies;
};


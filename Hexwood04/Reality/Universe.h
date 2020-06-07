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

	void AddStar(Star& star);
	void AddEmpire(Empire& empire);
	Star& GetStar(int index);
	int GetSize();
	bool Run();

private:
	std::map<int, Star> m_stars;
	std::mutex m_star_mutex;

	std::map<int, Empire> m_empires;
};


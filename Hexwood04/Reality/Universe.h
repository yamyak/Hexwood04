#pragma once

#include "Star.h"

#include <map>
#include <mutex>

class Universe
{
public:
	Universe();
	~Universe();

	void AddStar(Star& star);

private:
	std::map<int, Star> m_stars;
	std::mutex m_star_mutex;
};


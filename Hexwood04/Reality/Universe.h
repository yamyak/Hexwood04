#pragma once

#include "Star.h"

#include <vector>
#include <mutex>

class Universe
{
public:
	Universe();
	~Universe();

	void AddStar(Star& star);

private:
	std::vector<Star> m_stars;
	std::mutex m_star_list_mutex;
};


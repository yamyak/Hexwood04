#pragma once

#include "Object.h"
#include "Star.h"

#include <mutex>

class Universe : public Object
{
public:
	Universe();
	~Universe();

	void AddStar(Star& star);
	bool Run();

private:
	std::map<int, Star> m_stars;
	std::mutex m_star_mutex;
};


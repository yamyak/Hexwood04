#include "Universe.h"

Universe::Universe()
{

}

Universe::~Universe()
{

}

void Universe::AddStar(Star& star)
{
	std::lock_guard<std::mutex> guard(m_star_list_mutex);
	m_stars.push_back(star);
}

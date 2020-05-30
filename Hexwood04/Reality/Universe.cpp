#include "Universe.h"

Universe::Universe()
{

}

Universe::~Universe()
{

}

void Universe::AddStar(Star& star)
{
	std::lock_guard<std::mutex> guard(m_star_mutex);
	m_stars[star.GetId()] = star;
}

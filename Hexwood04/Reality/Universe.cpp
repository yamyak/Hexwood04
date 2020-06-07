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

bool Universe::Run()
{
	return true;
}

int Universe::GetSize()
{
	return (int)m_stars.size();
}

Star& Universe::GetStar(int index)
{
	return m_stars[index];
}

void Universe::AddEmpire(Empire& empire)
{
	m_empires[empire.GetId()] = empire;
}
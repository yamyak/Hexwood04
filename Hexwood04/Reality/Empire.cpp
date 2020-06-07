#include "Empire.h"

Empire::Empire(int id)
{
	SetId(id);
}

Empire::~Empire()
{

}

bool Empire::Run()
{
	return false;
}

void Empire::AddColony(Colony& colony)
{
	m_colonies[(int)m_colonies.size()] = colony;
}
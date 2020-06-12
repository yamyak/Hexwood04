#include "Empire.h"

Empire::Empire(int id)
{
	SetId(id);
}

Empire::~Empire()
{

}

bool Empire::Run(std::mutex& mutex, std::queue<Object*>& queue)
{
	return false;
}

void Empire::AddColony(Colony* colony)
{
	Lock();

	colony->SetEmpireId((int)m_colonies.size());
	m_colonies[colony->GetEmpireId()] = colony;

	Unlock();
}

std::map<int, Colony*>& Empire::GetColonies()
{
	return m_colonies;
}
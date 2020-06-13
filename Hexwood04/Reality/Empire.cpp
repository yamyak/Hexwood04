#include "Empire.h"

Empire::Empire(int id)
{
	SetId(id);
}

Empire::~Empire()
{

}

void Empire::Run(std::mutex& mutex, std::queue<Object*>& queue)
{
	Lock();

	for (auto& colony : m_colonies)
	{
		std::lock_guard<std::mutex> queue_lock(mutex);
		queue.push(static_cast<Object*>(colony.second));
	}

	Unlock();
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
#include "Empire.h"
#include "Colony.h"
#include "Ship.h"

Empire::Empire(int id)
{
	SetId(id);
}

Empire::~Empire()
{

}

void Empire::Run(std::mutex& mutex, std::queue<Object*>& queue)
{
	std::lock_guard<std::mutex> lock(m_object_mutex);
	//Lock();

	for (auto& colony : m_colonies)
	{
		std::lock_guard<std::mutex> queue_lock(mutex);
		queue.push(static_cast<Object*>(colony.second));
	}

	//Unlock();
}

void Empire::AddColony(Colony* colony)
{
	std::lock_guard<std::mutex> lock(m_object_mutex);
	//Lock();

	colony->SetEmpireId((int)m_colonies.size());
	m_colonies[colony->GetEmpireId()] = colony;

	//Unlock();
}

void Empire::AddShip(Ship* ship)
{
	std::lock_guard<std::mutex> lock(m_object_mutex);
	//Lock();

	ship->SetEmpireId((int)m_ships.size());
	m_ships[ship->GetEmpireId()] = ship;

	//Unlock();
}

std::vector<Colony*> Empire::GetColonies()
{
	std::lock_guard<std::mutex> lock(m_object_mutex);

	std::vector<Colony*> colonies;

	for (auto& co : m_colonies)
	{
		colonies.push_back(co.second);
	}

	return colonies;
}
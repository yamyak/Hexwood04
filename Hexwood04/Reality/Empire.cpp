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

	for (auto& ship : m_ships)
	{
		std::lock_guard<std::mutex> queue_lock(mutex);
		queue.push(static_cast<Object*>(ship.second));
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

void Empire::DeleteShip(int id)
{
	std::lock_guard<std::mutex> lock(m_object_mutex);

	m_ships.erase(id);
}

bool Empire::CheckOccupancy(ObjectType type, int id)
{
	std::lock_guard<std::mutex> lock(m_object_mutex);
	if (type == ObjectType::PLANET)
	{
		for (int& site : m_potenialColonySites)
		{
			if (site == id)
			{
				return false;
			}
		}
	}
	else if(type == ObjectType::STAR)
	{
		for (int& site : m_potentialEmpireSystems)
		{
			if (site == id)
			{
				return false;
			}
		}
	}

	return true;
}

void Empire::RegisterForOccupancy(ObjectType type, int id)
{
	std::lock_guard<std::mutex> lock(m_object_mutex);
	if (type == ObjectType::PLANET)
	{
		m_potenialColonySites.push_back(id);
	}
	else if (type == ObjectType::STAR)
	{
		m_potentialEmpireSystems.push_back(id);
	}
}

void Empire::CleanOccupanyRecords(ObjectType type, int id)
{
	std::lock_guard<std::mutex> lock(m_object_mutex);

	if (type == ObjectType::PLANET)
	{
		std::vector<int>::iterator iter = m_potenialColonySites.begin();
		while (iter != m_potenialColonySites.end())
		{
			if (*iter == id)
			{
				m_potenialColonySites.erase(iter);
				break;
			}

			iter++;
		}
	}
	else if (type == ObjectType::STAR)
	{
		std::vector<int>::iterator iter = m_potentialEmpireSystems.begin();
		while (iter != m_potentialEmpireSystems.end())
		{
			if (*iter == id)
			{
				m_potentialEmpireSystems.erase(iter);
				break;
			}

			iter++;
		}
	}
}
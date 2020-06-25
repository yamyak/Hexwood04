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
}

void Empire::AddColony(Colony* colony)
{
	std::lock_guard<std::mutex> lock(m_object_mutex);

	m_colonies[colony->GetId()] = colony;
}

void Empire::AddShip(Ship* ship)
{
	std::lock_guard<std::mutex> lock(m_object_mutex);

	m_ships[ship->GetId()] = ship;
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

std::vector<Ship*> Empire::GetShips()
{
	std::lock_guard<std::mutex> lock(m_object_mutex);

	std::vector<Ship*> ships;

	for (auto& ship : m_ships)
	{
		ships.push_back(ship.second);
	}

	return ships;
}

void Empire::DeleteShip(int id)
{
	std::lock_guard<std::mutex> lock(m_object_mutex);

	m_ships.erase(id);
}

bool Empire::CheckColonization(ObjectType type, int id)
{
	std::lock_guard<std::mutex> lock(m_object_mutex);

	std::map<int, int>& sites = (type == ObjectType::PLANET) ? m_potenialColonySites : m_potentialEmpireSystems;

	return sites.find(id) != sites.end();
}

bool Empire::SetColonized(ObjectType type, int id, int colony_id)
{
	std::lock_guard<std::mutex> lock(m_object_mutex);

	std::map<int, int>& sites = (type == ObjectType::PLANET) ? m_potenialColonySites : m_potentialEmpireSystems;

	if (sites.find(id) == sites.end())
	{
		sites[id] = colony_id;
		return true;
	}

	return false;
}
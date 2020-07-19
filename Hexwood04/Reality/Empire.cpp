#include "Empire.h"
#include "Colony.h"
#include "Ship.h"


/// <summary>
/// Constructor
/// </summary>
/// <param name="id">Empire id</param>
/// <returns></returns>
Empire::Empire(int id)
{
	SetId(id);
}

/// <summary>
/// Destructor
/// </summary>
/// <returns></returns>
Empire::~Empire()
{

}

/// <summary>
/// Empire processing method
/// </summary>
/// <param name="mutex">Processing queue mutex</param>
/// <param name="queue">Processing queue</param>
void Empire::Run(std::mutex& mutex, std::queue<Object*>& queue)
{
	std::lock_guard<std::mutex> lock(m_object_mutex);

	// iterate through all colonies in empire and process
	for (auto& colony : m_colonies)
	{
		std::lock_guard<std::mutex> queue_lock(mutex);
		queue.push(static_cast<Object*>(colony.second));
	}

	// iterate through all ships in empire and process
	for (auto& ship : m_ships)
	{
		std::lock_guard<std::mutex> queue_lock(mutex);
		queue.push(static_cast<Object*>(ship.second));
	}
}

/// <summary>
/// Add new colony to empire
/// </summary>
/// <param name="colony">Colony pointer</param>
void Empire::AddColony(Colony* colony)
{
	std::lock_guard<std::mutex> lock(m_object_mutex);

	m_colonies[colony->GetId()] = colony;
}

/// <summary>
/// Add new ship to empire
/// </summary>
/// <param name="ship">Ship pointer</param>
void Empire::AddShip(Ship* ship)
{
	std::lock_guard<std::mutex> lock(m_object_mutex);

	m_ships[ship->GetId()] = ship;
}

/// <summary>
/// Retrieve list of all colonies in empire
/// </summary>
/// <returns>List of all colonies in empire</returns>
std::vector<Colony*> Empire::GetColonies()
{
	std::lock_guard<std::mutex> lock(m_object_mutex);

	std::vector<Colony*> colonies;

	// copy of colony pointers to return vector
	for (auto& co : m_colonies)
	{
		colonies.push_back(co.second);
	}

	return colonies;
}

/// <summary>
/// Retrieve list of all ships in empire
/// </summary>
/// <returns>List of all ships in empire</returns>
std::vector<Ship*> Empire::GetShips()
{
	std::lock_guard<std::mutex> lock(m_object_mutex);

	std::vector<Ship*> ships;

	// copy of ship pointers to return vector
	for (auto& ship : m_ships)
	{
		ships.push_back(ship.second);
	}

	return ships;
}

/// <summary>
/// Delete ship from empire
/// </summary>
/// <param name="id">Ship id</param>
void Empire::DeleteShip(int id)
{
	std::lock_guard<std::mutex> lock(m_object_mutex);

	// only erases the map entry
	// does not delete the ship
	// universe object is responsible for deletion
	m_ships.erase(id);
}

/// <summary>
/// Check whether planet/star has already been flagged for colonization
/// </summary>
/// <param name="type">Object type: planet or star</param>
/// <param name="id">Object id</param>
/// <returns>Wehther object has been flagged for colonization</returns>
bool Empire::CheckColonization(ObjectType type, int id)
{
	std::lock_guard<std::mutex> lock(m_object_mutex);

	// retrieve the map for the type of object requested
	std::map<int, int>& sites = (type == ObjectType::PLANET) ? m_potenialColonySites : m_potentialEmpireSystems;

	// check whether object has already been flagged as a colonization target
	return sites.find(id) != sites.end();
}

/// <summary>
/// Set planet/star as a colonization target so won't be targeted by other friendly objects
/// </summary>
/// <param name="type">Object type: planet or star</param>
/// <param name="id">Object id</param>
/// <param name="colony_id">Id of colony trying to colonize this object</param>
/// <returns>Whether object was successful labelled a colonization target</returns>
bool Empire::SetColonized(ObjectType type, int id, int colony_id)
{
	std::lock_guard<std::mutex> lock(m_object_mutex);

	// retrieve the map for the type of object requested
	std::map<int, int>& sites = (type == ObjectType::PLANET) ? m_potenialColonySites : m_potentialEmpireSystems;

	// if object has not been flagged as a colonization target
	if (sites.find(id) == sites.end())
	{
		// flag it object and label it with the flagging colony id
		sites[id] = colony_id;
		return true;
	}

	return false;
}
#include "Universe.h"
#include "Star.h"
#include "Empire.h"
#include "Planet.h"
#include "Colony.h"
#include "Ship.h"


Universe* Universe::m_instance = nullptr;

/// <summary>
/// Constructor
/// </summary>
/// <returns></returns>
Universe::Universe() : m_age(0)
{
	CleanUp();
}

/// <summary>
/// Destructor
/// </summary>
/// <returns></returns>
Universe::~Universe()
{
	CleanUp();
	delete m_instance;
	m_instance = nullptr;
}

/// <summary>
/// Delete all objects in universe
/// </summary>
void Universe::CleanUp()
{
	std::lock_guard<std::mutex> lock(m_object_mutex);

	for (auto& colony : m_colonies)
	{
		if (colony.second != nullptr)
		{
			delete colony.second;
			colony.second = nullptr;
		}
	}

	for (auto& empire : m_empires)
	{
		if (empire.second != nullptr)
		{
			delete empire.second;
			empire.second = nullptr;
		}
	}

	for (auto& planet : m_planets)
	{
		if (planet.second != nullptr)
		{
			delete planet.second;
			planet.second = nullptr;
		}
	}

	for (auto& star : m_stars)
	{
		if (star.second != nullptr)
		{
			delete star.second;
			star.second = nullptr;
		}
	}

	for (auto& ship : m_ships)
	{
		if (ship.second != nullptr)
		{
			delete ship.second;
			ship.second = nullptr;
		}
	}
}

/// <summary>
/// Singleton accessor
/// </summary>
/// <returns>Pointer to Universe singleton</returns>
Universe* Universe::GetInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new Universe();
	}

	return m_instance;
}

/// <summary>
/// Add dead objects to graveyard
/// </summary>
/// <param name="type">Type of object</param>
/// <param name="id">Id of object</param>
void Universe::AddToGraveyard(ObjectType type, int id)
{
	std::lock_guard<std::mutex> lock(m_object_mutex);

	m_graveyard.push_back(std::make_pair(type, id));
}

/// <summary>
/// Add new star to universe
/// </summary>
/// <param name="ship">Star pointer to add</param>
void Universe::AddStar(Star* star)
{
	std::lock_guard<std::mutex> lock(m_object_mutex);

	m_stars[star->GetId()] = star;
}

/// <summary>
/// Add new planet to universe
/// </summary>
/// <param name="ship">Planet pointer to add</param>
void Universe::AddPlanet(Planet* planet)
{
	std::lock_guard<std::mutex> lock(m_object_mutex);

	m_planets[planet->GetId()] = planet;
}

/// <summary>
/// Add new empire to universe
/// </summary>
/// <param name="ship">Empire pointer to add</param>
void Universe::AddEmpire(Empire* empire)
{
	std::lock_guard<std::mutex> lock(m_object_mutex);

	m_empires[empire->GetId()] = empire;
}

/// <summary>
/// Add new colony to universe
/// </summary>
/// <param name="ship">Colony pointer to add</param>
void Universe::AddColony(Colony* colony)
{
	std::lock_guard<std::mutex> lock(m_object_mutex);

	m_colonies[colony->GetId()] = colony;
}

/// <summary>
/// Add new ship to universe
/// </summary>
/// <param name="ship">Ship pointer to add</param>
void Universe::AddShip(Ship* ship)
{
	std::lock_guard<std::mutex> lock(m_object_mutex);

	m_ships[ship->GetId()] = ship;
}

/// <summary>
/// Retrieves list of all objects of type requested
/// </summary>
/// <param name="type">Object type requested</param>
/// <returns>List of objects of type requested</returns>
std::vector<Object*> Universe::GetObjects(ObjectType type)
{
	std::vector<Object*> output;
	std::lock_guard<std::mutex> lock(m_object_mutex);

	// find the data structure for the requested object type
	// copy all pointers into new data structure which is returned
	switch (type)
	{
	case ObjectType::STAR:
	{
		output.reserve(m_stars.size());
		for (auto& star : m_stars)
		{
			output.push_back(star.second);
		}
		break;
	}
	case ObjectType::PLANET:
	{
		output.reserve(m_planets.size());
		for (auto& planet : m_planets)
		{
			output.push_back(planet.second);
		}
		break;
	}
	case ObjectType::EMPIRE:
	{
		output.reserve(m_empires.size());
		for (auto& empire : m_empires)
		{
			output.push_back(empire.second);
		}
		break;
	}
	case ObjectType::COLONY:
	{
		output.reserve(m_colonies.size());
		for (auto& colony : m_colonies)
		{
			output.push_back(colony.second);
		}
		break;
	}
	case ObjectType::SHIP:
	{
		output.reserve(m_ships.size());
		for (auto& ship : m_ships)
		{
			output.push_back(ship.second);
		}
		break;
	}
	default:
		break;
	}

	return output;
}

/// <summary>
/// Returns number of stars in universe
/// </summary>
/// <returns>Number of stars in universe</returns>
int Universe::GetStarCount()
{
	return (int)m_stars.size();
}

/// <summary>
/// Returns number of empires in universe
/// </summary>
/// <returns>Number of empires in universe</returns>
int Universe::GetEmpireCount()
{
	return (int)m_empires.size();
}

/// <summary>
/// Returns number of colonies in universe
/// </summary>
/// <returns>Number of colonies in universe</returns>
int Universe::GetColonyCount()
{
	return (int)m_colonies.size();
}

/// <summary>
/// Returns number of ships in universe
/// </summary>
/// <returns>Number of ships in universe</returns>
int Universe::GetShipCount()
{
	return (int)m_ships.size();
}

/// <summary>
/// Returns star associated with key provided
/// </summary>
/// <param name="key">Index into star database</param>
/// <returns>Star pointer</returns>
Star* Universe::GetStar(int key)
{
	Star* star = nullptr;
	if (m_stars.find(key) != m_stars.end())
	{
		star = m_stars[key];
	}

	return star;
}

/// <summary>
/// Delete all objects in graveyard
/// </summary>
void Universe::ClearOutGraveyard()
{
	std::lock_guard<std::mutex> lock(m_object_mutex);

	// iterate through all objects in graveyard
	for (auto& elem : m_graveyard)
	{
		switch (elem.first)
		{
			case ObjectType::SHIP:
			{
				Ship* ship = m_ships[elem.second];

				// erase ship from empire it belongs to
				ship->GetSourceColony()->GetEmpire()->DeleteShip(ship->GetId());

				// delete the ship
				m_ships.erase(elem.second);
				delete ship;
				ship = nullptr;

				break;
			}
			default:
				break;
		}
	}

	m_graveyard.clear();
}

/// <summary>
/// Increments age of universe
/// </summary>
void Universe::IncrementAge()
{
	m_age++;
}

/// <summary>
/// Returns age of universe
/// </summary>
/// <returns>Age of universe</returns>
int Universe::GetAge()
{
	return m_age;
}
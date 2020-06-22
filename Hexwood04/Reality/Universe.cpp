#include "Universe.h"
#include "Star.h"
#include "Empire.h"
#include "Planet.h"
#include "Colony.h"
#include "Ship.h"


Universe* Universe::m_instance = nullptr;

Universe::Universe() : m_age(0)
{
	CleanUp();
}

Universe::~Universe()
{
	CleanUp();
	delete m_instance;
	m_instance = nullptr;
}

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

Universe* Universe::GetInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new Universe();
	}

	return m_instance;
}

void Universe::AddToGraveyard(ObjectType type, int id)
{
	std::lock_guard<std::mutex> lock(m_object_mutex);

	m_graveyard.push_back(std::make_pair(type, id));
}

void Universe::AddStar(Star* star)
{
	std::lock_guard<std::mutex> lock(m_object_mutex);

	m_stars[star->GetId()] = star;
}

void Universe::AddPlanet(Planet* planet)
{
	std::lock_guard<std::mutex> lock(m_object_mutex);

	m_planets[planet->GetId()] = planet;
}

void Universe::AddEmpire(Empire* empire)
{
	std::lock_guard<std::mutex> lock(m_object_mutex);

	m_empires[empire->GetId()] = empire;
}

void Universe::AddColony(Colony* colony)
{
	std::lock_guard<std::mutex> lock(m_object_mutex);

	m_colonies[colony->GetId()] = colony;
}

void Universe::AddShip(Ship* ship)
{
	std::lock_guard<std::mutex> lock(m_object_mutex);

	m_ships[ship->GetId()] = ship;
}

std::vector<Object*> Universe::GetObjects(ObjectType type)
{
	std::vector<Object*> output;
	std::lock_guard<std::mutex> lock(m_object_mutex);

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

int Universe::GetStarCount()
{
	return (int)m_stars.size();
}

int Universe::GetEmpireCount()
{
	return (int)m_empires.size();
}

int Universe::GetColonyCount()
{
	return (int)m_colonies.size();
}

int Universe::GetShipCount()
{
	return (int)m_ships.size();
}

Star* Universe::GetStar(int key)
{
	Star* star = nullptr;
	if (m_stars.find(key) != m_stars.end())
	{
		star = m_stars[key];
	}

	return star;
}

void Universe::ClearOutGraveyard()
{
	std::lock_guard<std::mutex> lock(m_object_mutex);

	for (auto& elem : m_graveyard)
	{
		switch (elem.first)
		{
			case ObjectType::SHIP:
			{
				Ship* ship = m_ships[elem.second];

				ship->GetSourceColony()->GetEmpire()->DeleteShip(ship->GetId());

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

void Universe::IncrementAge()
{
	m_age++;
}

int Universe::GetAge()
{
	return m_age;
}
#include "Colony.h"
#include "Planet.h"
#include "Ship.h"
#include "Empire.h"
#include "Universe.h"
#include "Star.h"

#include <cmath>


std::atomic<int> Colony::m_global_id = 0;

/// <summary>
/// Constructor
/// </summary>
/// <param name="planet">Parent planet pointer</param>
/// <param name="empire">Associated empire pointer</param>
/// <param name="period_lengths">Map of time period lengths</param>
/// <param name="rates">Map of resource consumption rates</param>
/// <returns></returns>
Colony::Colony(Planet* planet, Empire* empire, std::map<CivilizationPeriod, int> period_lengths,
	std::map<CivilizationPeriod, std::map<ResourceType, float>> rates) : m_planet(planet), 
	m_period_lengths(period_lengths), m_consumption_rates(rates), m_current_period(CivilizationPeriod::PREHISTORY),
	m_age(0), m_empire(empire)
{
	SetId(m_global_id++);
}

/// <summary>
/// Copy constructor
/// </summary>
/// <param name="colony">Colony pointer to be copied</param>
/// <returns></returns>
Colony::Colony(Colony* colony) : m_planet(nullptr),
	m_period_lengths(colony->m_period_lengths), m_consumption_rates(colony->m_consumption_rates), 
	m_current_period(CivilizationPeriod::SPACE_AGE), m_age(0), m_empire(colony->m_empire)
{
	SetId(m_global_id++);
}

/// <summary>
/// Destructor
/// </summary>
/// <returns></returns>
Colony::~Colony()
{

}

/// <summary>
/// Set host planet
/// </summary>
/// <param name="planet">Planet pointer</param>
void Colony::SetPlanet(Planet* planet)
{
	m_planet = planet;
}

/// <summary>
/// Get associated empire
/// </summary>
/// <returns>Empire pointer</returns>
Empire* Colony::GetEmpire()
{
	return m_empire;
}

/// <summary>
/// Get host planet
/// </summary>
/// <returns>Planet pointer</returns>
Planet* Colony::GetPlanet()
{
	return m_planet;
}

/// <summary>
/// Colony processing method
/// </summary>
/// <param name="mutex">Processing queue mutex</param>
/// <param name="queue">Processing queue</param>
void Colony::Run(std::mutex& mutex, std::queue<Object*>& queue)
{
	std::lock_guard<std::mutex> lock(m_object_mutex);

	// collect resources from host planet
	std::map<ResourceType, float> collected = m_planet->CollectResources(m_consumption_rates[m_current_period]);

	bool collection_failed = false;
	for(auto& item : collected)
	{
		// if any resource was not able to be collected, colony does not grow this time frame
		if (item.second == 0 && m_consumption_rates[m_current_period][item.first] != 0)
		{
			collection_failed = true;
			break;
		}
	}

	// if resource were collected
	if (!collection_failed)
	{
		m_age++;
		// check if colony is old enough to evolve to next period
		// if space age, instead of evolving, will create a new colonization ship
		if (m_age >= m_period_lengths[m_current_period])
		{
			m_age = 0;

			// if not in space, evolve to next period
			if (m_current_period != CivilizationPeriod::SPACE_AGE)
			{
				m_current_period = (CivilizationPeriod)((int)m_current_period + 1);
			}
			else
			{
				bool planet = false;
				Object* obj = nullptr;
				// search for a place to colonize
				if (FindNextColonyLocation(planet, obj))
				{
					// create a ship to send to colonization target
					Ship* ship = new Ship(this, LIGHT_SPEED, !planet, obj);
					Universe::GetInstance()->AddShip(ship);
					m_empire->AddShip(ship);
				}
			}
		}
	}

	// add host planet to processing queue
	std::lock_guard<std::mutex> queue_lock(mutex);
	queue.push(static_cast<Object*>(m_planet));
}

/// <summary>
/// Find a colonization target
/// </summary>
/// <param name="planet">Whether target is nearby planet or far away star</param>
/// <param name="obj">Colonization target pointer</param>
/// <returns>Whether target was found</returns>
bool Colony::FindNextColonyLocation(bool& planet, Object*& obj)
{
	bool found = false;

	planet = false;
	obj = nullptr;

	// check local system first
	std::vector<Planet*> planets = m_planet->GetStar()->GetSystem();
	for (Planet* pl : planets)
	{
		// if not host planet and able to set as colonization target
		if (pl->GetId() != m_planet->GetId() && m_empire->SetColonized(ObjectType::PLANET, pl->GetId(), GetId()))
		{
			// return planet found
			planet = true;
			obj = pl;
			found = true;
			break;
		}
	}

	if (!found)
	{
		// search for other star systems
		found = DeepSkySearch(obj);
	}

	return found;
}

/// <summary>
/// Find a colonization target in a different star system
/// </summary>
/// <param name="obj">Colonization target pointer</param>
/// <returns>Whether target was found</returns>
bool Colony::DeepSkySearch(Object*& obj)
{
	std::vector<Object*> star_objs = Universe::GetInstance()->GetObjects(ObjectType::STAR);

	float x1, y1, z1;
	std::tie(x1, y1, z1) = m_planet->GetStar()->GetLocation();

	float distance = std::numeric_limits<float>::max();
	obj = nullptr;

	Star* star = nullptr;

	// iterate through all stars in universe
	for (Object* star_obj : star_objs)
	{
		// if not current system and not slated for colonization
		if (star_obj->GetId() != m_planet->GetStar()->GetId() && !m_empire->CheckColonization(ObjectType::STAR, star_obj->GetId()))
		{
			// get target location
			Star* candidate = static_cast<Star*>(star_obj);
			float x2, y2, z2;
			std::tie(x2, y2, z2) = candidate->GetLocation();

			// get distance to target
			float dist = std::sqrt(std::pow((x2 - x1), 2) + std::pow((y2 - y1), 2) + std::pow((z2 - z1), 2));
			// find star that is closest to host star
			if (distance > dist)
			{
				distance = dist;
				star = candidate;
			}
		}
	}

	// if able to set as colonization target, return star pointer
	if (star != nullptr && m_empire->SetColonized(ObjectType::STAR, star->GetId(), GetId()))
	{
		obj = star;
	}

	return obj != nullptr;
}
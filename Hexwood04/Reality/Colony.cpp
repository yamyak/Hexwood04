#include "Colony.h"
#include "Planet.h"
#include "Ship.h"
#include "Empire.h"
#include "Universe.h"
#include "Star.h"

#include <cmath>


std::atomic<int> Colony::m_global_id = 0;

Colony::Colony(Planet* planet, Empire* empire, std::map<CivilizationPeriod, int> period_lengths,
	std::map<CivilizationPeriod, std::map<ResourceType, float>> rates) : m_empire_id(0), m_planet(planet), 
	m_period_lengths(period_lengths), m_consumption_rates(rates), m_current_period(CivilizationPeriod::PREHISTORY),
	m_age(0), m_empire(empire)
{
	SetId(m_global_id++);
}

Colony::Colony(Colony* colony) : m_empire_id(0), m_planet(nullptr),
	m_period_lengths(colony->m_period_lengths), m_consumption_rates(colony->m_consumption_rates), 
	m_current_period(CivilizationPeriod::SPACE_AGE), m_age(0), m_empire(colony->m_empire)
{
	SetId(m_global_id++);
}

Colony::~Colony()
{

}

void Colony::SetPlanet(Planet* planet)
{
	m_planet = planet;
}

Empire* Colony::GetEmpire()
{
	return m_empire;
}

Planet* Colony::GetPlanet()
{
	return m_planet;
}

void Colony::Run(std::mutex& mutex, std::queue<Object*>& queue)
{
	std::lock_guard<std::mutex> lock(m_object_mutex);
	//Lock();

	std::map<ResourceType, float> collected = m_planet->CollectResources(m_consumption_rates[m_current_period]);

	bool collection_failed = false;
	for(auto& item : collected)
	{
		if (item.second == 0 && m_consumption_rates[m_current_period][item.first] != 0)
		{
			collection_failed = true;
			break;
		}
	}

	if (!collection_failed)
	{
		m_age++;
		if (m_age > m_period_lengths[m_current_period])
		{
			m_age = 0;

			if (m_current_period != CivilizationPeriod::SPACE_AGE)
			{
				m_current_period = (CivilizationPeriod)((int)m_current_period + 1);
			}
			else
			{
				bool planet = false;
				Object* obj = nullptr;
				if (FindNextColonyLocation(planet, obj))
				{
					Ship* ship = new Ship(this, LIGHT_SPEED, planet, obj);
					Universe::GetInstance()->AddShip(ship);
					m_empire->AddShip(ship);
				}
			}
		}
	}

	//Unlock();
}

int Colony::GetEmpireId()
{
	return m_empire_id;
}

void Colony::SetEmpireId(int id)
{
	m_empire_id = id;
}

bool Colony::FindNextColonyLocation(bool& planet, Object* obj)
{
	bool found = false;

	planet = false;
	obj = nullptr;

	std::vector<Planet*> planets = m_planet->GetStar()->GetSystem();
	for (auto& pl : planets)
	{
		if (pl->GetId() != GetId() && !pl->GetOccupied())
		{
			planet = true;
			obj = pl;
			found = true;
		}
	}

	if (!found)
	{
		found = DeepSkySearch(obj);
	}

	return found;
}

bool Colony::DeepSkySearch(Object* obj)
{
	std::vector<Object*> star_objs = Universe::GetInstance()->GetObjects(ObjectType::STAR);

	float x1, y1, z1;
	std::tie(x1, y1, z1) = m_planet->GetStar()->GetLocation();

	float distance = std::numeric_limits<float>::max();
	obj = nullptr;

	for (Object* obj : star_objs)
	{
		Star* star = static_cast<Star*>(obj);
		float x2, y2, z2;
		std::tie(x2, y2, z2) = star->GetLocation();

		float dist = std::sqrt(std::pow((x2 - x1), 2) + std::pow((y2 - y1), 2) + std::pow((z2 - z1), 2));
		if (distance > dist)
		{
			distance = dist;
			obj = star;
		}
	}

	return obj != nullptr;
}
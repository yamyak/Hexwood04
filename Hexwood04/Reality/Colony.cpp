#include "Colony.h"
#include "Planet.h"
#include "Ship.h"
#include "Empire.h"

std::atomic<int> Colony::m_global_id = 0;

Colony::Colony(Planet* planet, Empire* empire, std::map<CivilizationPeriod, int> period_lengths,
	std::map<CivilizationPeriod, std::map<ResourceType, float>> rates) : m_empire_id(0), m_planet(planet), 
	m_period_lengths(period_lengths), m_consumption_rates(rates), m_current_period(CivilizationPeriod::PREHISTORY),
	m_age(0), m_empire(empire)
{
	SetId(m_global_id++);
}

Colony::~Colony()
{

}

void Colony::Run(std::mutex& mutex, std::queue<Object*>& queue)
{
	std::lock_guard<std::mutex> lock(m_object_mutex);
	//Lock();

	std::map<ResourceType, float> collected = m_planet->CollectResources(m_consumption_rates[m_current_period]);

	bool collection_failed = false;
	std::map<ResourceType, float>::iterator it = collected.begin();
	while (it != collected.end())
	{
		if (it->second == 0 && m_consumption_rates[m_current_period][it->first] != 0)
		{
			collection_failed = true;
			break;
		}
	}

	if (!collection_failed)
	{
		int period_length = m_period_lengths[m_current_period];
		
		m_age++;
		if (m_age > period_length)
		{
			m_age = 0;

			if (m_current_period != CivilizationPeriod::SPACE_AGE)
			{
				m_current_period = (CivilizationPeriod)((int)m_current_period + 1);
			}
			else
			{
				Ship* ship = new Ship();
				m_empire->AddShip(ship);
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
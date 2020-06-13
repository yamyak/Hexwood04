#include "Colony.h"

std::atomic<int> Colony::m_global_id = 0;

Colony::Colony(Planet* planet, std::map<CivilizationPeriod, int> period_lengths, 
	std::map<CivilizationPeriod, std::map<ResourceType, float>> rates) : m_empire_id(0), m_planet(planet), 
	m_period_lengths(period_lengths), m_consumption_rates(rates), m_current_period(CivilizationPeriod::PREHISTORY)
{
	SetId(m_global_id++);
}

Colony::~Colony()
{

}

void Colony::Run(std::mutex& mutex, std::queue<Object*>& queue)
{

}

int Colony::GetEmpireId()
{
	return m_empire_id;
}

void Colony::SetEmpireId(int id)
{
	m_empire_id = id;
}
#include "LifeEngine.h"
#include "../Utilities/ConfigReader.h"


LifeEngine::LifeEngine(int seed)
{
	m_resource_consumption = ConfigReader::GetInstance()->GetFloatArray(Constants::COLONY, Constants::RESOURCE_CONSUMPTION);
	m_period_length = ConfigReader::GetInstance()->GetIntArray(Constants::COLONY, Constants::COLONY_PERIOD_LENGTH);
	m_period_sigma = ConfigReader::GetInstance()->GetIntArray(Constants::COLONY, Constants::COLONY_PERIOD_LENGTH_SIGMA);

	m_generator.seed(seed);
	m_uniform_dist.param(std::uniform_int_distribution<int>::param_type(0, std::numeric_limits<int>::max()));
	m_empire_normal_dist.param(std::normal_distribution<float>::param_type(ConfigReader::GetInstance()->GetFloat(Constants::EMPIRE, Constants::AVERAGE_EMPIRES),
		ConfigReader::GetInstance()->GetFloat(Constants::EMPIRE, Constants::EMPIRE_SIGMA)));
}

LifeEngine::~LifeEngine()
{

}

bool LifeEngine::CreateEmpires(Universe& verse)
{
	int number_empires = (int)m_empire_normal_dist(m_generator);
	int number_stars = verse.GetSize();
	int number_planets = 0;
	int index = 0;

	for (int i = 0; i < number_empires; i++)
	{
		bool planet_found = false;
		while (!planet_found)
		{
			index = m_uniform_dist(m_generator) % number_stars;
			Star* star = verse.GetStar(index);

			number_planets = star->GetSystemSize();
			if (number_planets > 0)
			{
				index = m_uniform_dist(m_generator) % number_planets;
				Planet* planet = star->GetPlanet(index);

				if (planet->SetOccupied())
				{
					Empire* empire = new Empire(i);
					
					std::map<CivilizationPeriod, int> periods = CalculatePeriodLengths();
					std::map<CivilizationPeriod, std::map<ResourceType, float>> rates = CalculateConsumptionRates();

					Colony* colony = new Colony(planet, periods, rates);
					verse.AddColony(colony);
					empire->AddColony(colony);

					verse.AddEmpire(empire);

					planet_found = true;
				}
			}
		}
	}

	return true;
}

std::map<CivilizationPeriod, int> LifeEngine::CalculatePeriodLengths()
{
	std::map<CivilizationPeriod, int> periods;
	for (CivilizationPeriod i = CivilizationPeriod::PREHISTORY; i < CivilizationPeriod::PERIOD_MAX; i = CivilizationPeriod((int)i + 1))
	{
		int base = m_period_length[(int)i][0];
		int sigma = m_period_sigma[(int)i][0];

		std::normal_distribution<float> period_normal_dist((float)base, (float)sigma);
		int period = (int)period_normal_dist(m_generator);
		periods[i] = (period < 0) ? 0 : period;
	}

	return periods;
}

std::map<CivilizationPeriod, std::map<ResourceType, float>> LifeEngine::CalculateConsumptionRates()
{
	std::map<CivilizationPeriod, std::map<ResourceType, float>> rates;

	for (ResourceType i = ResourceType::NATURAL_GAS; i < ResourceType::RESOURCE_MAX; i = ResourceType((int)i + 1))
	{
		for (CivilizationPeriod j = CivilizationPeriod::PREHISTORY; j < CivilizationPeriod::PERIOD_MAX; j = CivilizationPeriod((int)j + 1))
		{
			float rate = 0;
			float max = m_resource_consumption[(int)i][(int)j];
			if (max > 0)
			{
				std::uniform_real_distribution<float> period_uniform_dist(0, max);
				rate = period_uniform_dist(m_generator);
			}
			
			rates[j][i] = rate;
		}
	}

	return rates;
}
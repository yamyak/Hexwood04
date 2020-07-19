#include "LifeEngine.h"
#include "../Utilities/ConfigReader.h"
#include "../Reality/Universe.h"
#include "../Reality/Star.h"
#include "../Reality/Planet.h"
#include "../Reality/Empire.h"
#include "../Reality/Colony.h"


/// <summary>
/// Constructor
/// </summary>
/// <param name="seed">RNG seed</param>
/// <returns></returns>
LifeEngine::LifeEngine(int seed)
{
	// read initialization values from configuration file
	m_resource_consumption = ConfigReader::GetInstance()->GetFloatArray(Constants::COLONY, Constants::RESOURCE_CONSUMPTION);
	m_period_length = ConfigReader::GetInstance()->GetIntArray(Constants::COLONY, Constants::COLONY_PERIOD_LENGTH);
	m_period_sigma = ConfigReader::GetInstance()->GetIntArray(Constants::COLONY, Constants::COLONY_PERIOD_LENGTH_SIGMA);
	// initialize the RNG distribution parameters
	m_uniform_dist.param(std::uniform_int_distribution<int>::param_type(0, std::numeric_limits<int>::max()));
	m_empire_normal_dist.param(std::normal_distribution<float>::param_type(ConfigReader::GetInstance()->GetFloat(Constants::EMPIRE, Constants::AVERAGE_EMPIRES),
		ConfigReader::GetInstance()->GetFloat(Constants::EMPIRE, Constants::EMPIRE_SIGMA)));

	// initialize the RNG
	m_generator.seed(seed);
}

/// <summary>
/// Destructor
/// </summary>
/// <returns></returns>
LifeEngine::~LifeEngine()
{

}

/// <summary>
/// Create empires 
/// </summary>
/// <returns>Whether empires were created without errors</returns>
bool LifeEngine::CreateEmpires()
{
	// determine number of empires in universe
	int number_empires = (int)m_empire_normal_dist(m_generator);
	// make sure there is at least 1 empire
	number_empires = number_empires < 1 ? 1 : number_empires;
	int number_stars = Universe::GetInstance()->GetStarCount();
	int number_planets = 0;
	int index = 0;

	for (int i = 0; i < number_empires; i++)
	{
		// continue searching until a home planet is found
		bool planet_found = false;
		while (!planet_found)
		{
			// randomly select a star
			index = m_uniform_dist(m_generator) % number_stars;
			Star* star = Universe::GetInstance()->GetStar(index);

			// make sure that star exists
			// index id system for stars is based on csv file so no guarantee
			if (star != nullptr)
			{
				// make sure there is a planet to start a civilization on
				number_planets = star->GetSystemSize();
				if (number_planets > 0)
				{
					// randomly select a planet is system
					index = m_uniform_dist(m_generator) % number_planets;
					Planet* planet = star->GetPlanet(index);

					// if planet is a gas giant and not occupied, set it as occupied
					if (planet->GetPlanetType() != PlanetType::GAS_GIANT && planet->SetOccupied())
					{
						Empire* empire = new Empire(i);

						// initialize time period lengths and resource consumption rates during those periods
						std::map<CivilizationPeriod, int> periods = CalculatePeriodLengths();
						std::map<CivilizationPeriod, std::map<ResourceType, float>> rates = CalculateConsumptionRates();

						Colony* colony = new Colony(planet, empire, periods, rates);
						// add colony to universe
						Universe::GetInstance()->AddColony(colony);
						// add reference to colony to empire
						empire->AddColony(colony);
						// set the planet and star as colonized
						// no other empires can start in this system or on this planet
						empire->SetColonized(ObjectType::STAR, star->GetId(), colony->GetId());
						empire->SetColonized(ObjectType::PLANET, planet->GetId(), colony->GetId());

						// add empire to universe
						Universe::GetInstance()->AddEmpire(empire);

						planet_found = true;
					}
				}
			}
		}
	}

	return true;
}

/// <summary>
/// Randomly generate the time period lengths 
/// </summary>
/// <returns>Map of time period lengths</returns>
std::map<CivilizationPeriod, int> LifeEngine::CalculatePeriodLengths()
{
	// iterate through all time periods
	std::map<CivilizationPeriod, int> periods;
	for (CivilizationPeriod i = CivilizationPeriod::PREHISTORY; i < CivilizationPeriod::PERIOD_MAX; i = CivilizationPeriod((int)i + 1))
	{
		// get the base and sigma values for normal distribution for time period
		int base = m_period_length[(int)i][0];
		int sigma = m_period_sigma[(int)i][0];

		// set up the distribution with parameters and generate period length
		std::normal_distribution<float> period_normal_dist((float)base, (float)sigma);
		int period = (int)period_normal_dist(m_generator);
		// make sure period length is at least 1 period long
		periods[i] = (period < 1) ? 1 : period;
	}

	return periods;
}

/// <summary>
/// Randomly generate the consumption rate for each time period 
/// </summary>
/// <returns>Maps of time period resource consumption rates</returns>
std::map<CivilizationPeriod, std::map<ResourceType, float>> LifeEngine::CalculateConsumptionRates()
{
	std::map<CivilizationPeriod, std::map<ResourceType, float>> rates;

	// iterate through all resources
	for (ResourceType i = ResourceType::NATURAL_GAS; i < ResourceType::RESOURCE_MAX; i = ResourceType((int)i + 1))
	{
		// iterate through all time periods
		for (CivilizationPeriod j = CivilizationPeriod::PREHISTORY; j < CivilizationPeriod::PERIOD_MAX; j = CivilizationPeriod((int)j + 1))
		{
			// get maximum resource consumption allowed for period and resource
			float rate = 0;
			float max = m_resource_consumption[(int)i][(int)j];
			if (max > 0)
			{
				// set the resource consumption rate during this period
				std::uniform_real_distribution<float> period_uniform_dist(0, max);
				rate = period_uniform_dist(m_generator);
			}
			
			rates[j][i] = rate;
		}
	}

	return rates;
}
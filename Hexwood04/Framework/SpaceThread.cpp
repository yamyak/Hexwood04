#include "SpaceThread.h"
#include "../Utilities/ConfigReader.h"
#include "../Reality/Star.h"
#include "../Reality/Universe.h"


/// <summary>
/// Constructor
/// </summary>
/// <param name="seed">RNG seed</param>
/// <returns></returns>
SpaceThread::SpaceThread(int seed)
{
	// read initialization values from configuration file
	m_max_planets = ConfigReader::GetInstance()->GetInt(Constants::SOLAR_SYSTEM, Constants::MAX_PLANETS);
	m_gas_giant_min_size = ConfigReader::GetInstance()->GetFloat(Constants::SOLAR_SYSTEM, Constants::GAS_GIANT_MIN_SIZE);
	m_gas_giant_max_size = ConfigReader::GetInstance()->GetFloat(Constants::SOLAR_SYSTEM, Constants::GAS_GIANT_MAX_SIZE);
	m_terrestrial_min_size = ConfigReader::GetInstance()->GetFloat(Constants::SOLAR_SYSTEM, Constants::TERRESTRIAL_MIN_SIZE);
	m_terrestrial_max_size = ConfigReader::GetInstance()->GetFloat(Constants::SOLAR_SYSTEM, Constants::TERRESTRIAL_MAX_SIZE);
	m_planet_resources = ConfigReader::GetInstance()->GetFloatArray(Constants::SOLAR_SYSTEM, Constants::PLANET_RESOURCES);
	m_resource_modifiers = ConfigReader::GetInstance()->GetFloatArray(Constants::SOLAR_SYSTEM, Constants::RESOURCE_MODIFIERS);
	m_resource_recharge_rates = ConfigReader::GetInstance()->GetFloatArray(Constants::SOLAR_SYSTEM, Constants::RESOURCE_RECHARGE_RATE);
	// initialize the resource distribution parameters for RNG
	m_resource_normal_dist.param(std::normal_distribution<float>::param_type(0, ConfigReader::GetInstance()->GetFloat(Constants::SOLAR_SYSTEM, Constants::RESOURCE_SIGMA)));
	m_recharge_normal_dist.param(std::normal_distribution<float>::param_type(0, ConfigReader::GetInstance()->GetFloat(Constants::SOLAR_SYSTEM, Constants::RECHARGE_RATE_SIGMA)));

	// initialize the RNG
	m_generator.seed(seed);
	m_uniform_dist.param(std::uniform_int_distribution<int>::param_type(0, std::numeric_limits<int>::max()));
}

/// <summary>
/// Destructor
/// </summary>
/// <returns></returns>
SpaceThread::~SpaceThread()
{

}

/// <summary>
/// Populate the universe Star by star 
/// </summary>
/// <param name="starDB">Star database</param>
/// <param name="start">Start index into star database</param>
/// <param name="end">End index into star database</param>
/// <returns>Whether universe was populated without errors</returns>
bool SpaceThread::CreateUniverse(StarTable& starDB, int start, int end)
{
	for (int i = start; i < end; i++)
	{
		StarData data = starDB.GetStar(i);

		Star* star = CreateSystem(data);

		Universe::GetInstance()->AddStar(star);
	}

	return true;
}

/// <summary>
/// Create star and planets associated with it 
/// </summary>
/// <param name="data">Star database</param>
/// <returns>Star pointer</returns>
Star* SpaceThread::CreateSystem(StarData& data)
{
	Star* star = new Star(data.id, data.name, data.x, data.y, data.z);

	// determine number of planets in system 
	int planet_number = m_uniform_dist(m_generator) % m_max_planets;

	for (int i = 0; i < planet_number; i++)
	{
		// determine type of current planet
		PlanetType type = (PlanetType)(m_uniform_dist(m_generator) % (int)PlanetType::TYPE_MAX);

		// if planet is terrestrial, determine environment of planet
		PlanetEnvironment env = PlanetEnvironment::NONE;
		if (type == PlanetType::TERRESTRIAL)
		{
			env = (PlanetEnvironment)(m_uniform_dist(m_generator) % ((int)PlanetEnvironment::ENVIRONMENT_MAX - 1) + 1);
		}

		// create planet resources
		std::map<ResourceType, Resource> resources;
		resources[ResourceType::NATURAL_GAS] = CreateResource(ResourceType::NATURAL_GAS, type, env);
		resources[ResourceType::BIOMASS] = CreateResource(ResourceType::BIOMASS, type, env);
		resources[ResourceType::FOSSIL_FUELS] = CreateResource(ResourceType::FOSSIL_FUELS, type, env);
		resources[ResourceType::HEAVY_METALS] = CreateResource(ResourceType::HEAVY_METALS, type, env);
		resources[ResourceType::WATER] = CreateResource(ResourceType::WATER, type, env);
		resources[ResourceType::RADIOACTIVE] = CreateResource(ResourceType::RADIOACTIVE, type, env);

		Planet* planet = new Planet(type, env, star, resources);
		// add planet to universe
		Universe::GetInstance()->AddPlanet(planet);

		// add planet reference to star
		star->AddPlanet(planet);
	}

	return star;
}

/// <summary>
/// Create resource value for a planet 
/// </summary>
/// <param name="type">Resource type</param>
/// <param name="planet">Planet type: terrestrial or gas giant</param>
/// <param name="env">Planet environment</param>
/// <returns>Resource object</returns>
Resource SpaceThread::CreateResource(ResourceType type, PlanetType planet, PlanetEnvironment env)
{
	// get base resource value
	// applied modifier to base value
	float base = m_planet_resources[(int)type][(int)planet];
	base *= (1 + m_resource_normal_dist(m_generator) / 100);

	if (planet == PlanetType::TERRESTRIAL)
	{
		// if terrestrial planet, get environment modifier
		float mod = m_resource_modifiers[(int)type][(size_t)env - 1];
		// applied additional modifier to environment modifier
		mod *= (1 + m_resource_normal_dist(m_generator) / 100);
		// apply environment modifier
		base *= (1 + mod / 100);
	}

	// if final value is negative, bound it to zero
	base = (base < 0) ? 0 : base;

	// only set recharge rate if base greater than zero
	float recharge_rate = 0;
	if (base > 0)
	{
		// get recharge rate based on planet environment
		recharge_rate = m_resource_recharge_rates[(int)type][(int)env];
		if (recharge_rate > 0)
		{
			// apply recharge modifier to recharge rate
			recharge_rate *= (1 + m_recharge_normal_dist(m_generator) / 100);

			// make sure recharge rate is non-negative
			recharge_rate = (recharge_rate < 0) ? 0 : recharge_rate;
		}
	}

	Resource r;
	r.m_type = type;
	r.m_current = base;
	r.m_max = r.m_current;
	r.recharge_rate = recharge_rate;

	return r;
}
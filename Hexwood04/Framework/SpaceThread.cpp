#include "SpaceThread.h"
#include "../Utilities/ConfigReader.h"


//using namespace Constants;

SpaceThread::SpaceThread(int seed)
{
	m_max_planets = ConfigReader::GetInstance()->GetInt(Constants::SOLAR_SYSTEM, Constants::MAX_PLANETS);
	m_gas_giant_min_size = ConfigReader::GetInstance()->GetFloat(Constants::SOLAR_SYSTEM, Constants::GAS_GIANT_MIN_SIZE);
	m_gas_giant_max_size = ConfigReader::GetInstance()->GetFloat(Constants::SOLAR_SYSTEM, Constants::GAS_GIANT_MAX_SIZE);
	m_terrestrial_min_size = ConfigReader::GetInstance()->GetFloat(Constants::SOLAR_SYSTEM, Constants::TERRESTRIAL_MIN_SIZE);
	m_terrestrial_max_size = ConfigReader::GetInstance()->GetFloat(Constants::SOLAR_SYSTEM, Constants::TERRESTRIAL_MAX_SIZE);
	m_planet_resources = ConfigReader::GetInstance()->GetFloatArray(Constants::SOLAR_SYSTEM, Constants::PLANET_RESOURCES);
	m_resource_modifiers = ConfigReader::GetInstance()->GetFloatArray(Constants::SOLAR_SYSTEM, Constants::RESOURCE_MODIFIERS);
	m_resource_recharge_rates = ConfigReader::GetInstance()->GetFloatArray(Constants::SOLAR_SYSTEM, Constants::RESOURCE_RECHARGE_RATE);
	m_generator.seed(seed);
	m_uniform_dist.param(std::uniform_int_distribution<int>::param_type(0, std::numeric_limits<int>::max()));
	m_resource_normal_dist.param(std::normal_distribution<float>::param_type(0, ConfigReader::GetInstance()->GetFloat(Constants::SOLAR_SYSTEM, Constants::RESOURCE_SIGMA)));
	m_recharge_normal_dist.param(std::normal_distribution<float>::param_type(0, ConfigReader::GetInstance()->GetFloat(Constants::SOLAR_SYSTEM, Constants::RECHARGE_RATE_SIGMA)));
}

SpaceThread::~SpaceThread()
{

}

bool SpaceThread::CreateUniverse(Universe& verse, StarTable& starDB, int start, int end)
{
	for (int i = start; i < end; i++)
	{
		StarData data = starDB.GetStar(i);

		Star* star = CreateSystem(verse, data);

		verse.AddStar(star);
	}

	return true;
}

Star* SpaceThread::CreateSystem(Universe& verse, StarData& data)
{
	Star* star = new Star(data.id, data.name, data.x, data.y, data.z);

	int planet_number = m_uniform_dist(m_generator) % m_max_planets;

	for (int i = 0; i < planet_number; i++)
	{
		PlanetType type = (PlanetType)(m_uniform_dist(m_generator) % (int)PlanetType::TYPE_MAX);

		PlanetEnvironment env = PlanetEnvironment::NONE;
		if (type == PlanetType::TERRESTRIAL)
		{
			env = (PlanetEnvironment)(m_uniform_dist(m_generator) % ((int)PlanetEnvironment::ENVIRONMENT_MAX - 1) + 1);
		}

		std::vector<Resource> resources;
		resources.push_back(CreateResource(ResourceType::NATURAL_GAS, type, env));
		resources.push_back(CreateResource(ResourceType::BIOMASS, type, env));
		resources.push_back(CreateResource(ResourceType::FOSSIL_FUELS, type, env));
		resources.push_back(CreateResource(ResourceType::HEAVY_METALS, type, env));
		resources.push_back(CreateResource(ResourceType::WATER, type, env));
		resources.push_back(CreateResource(ResourceType::RADIOACTIVE, type, env));

		Planet* planet = new Planet(i, type, env, resources);
		verse.AddPlanet(planet);

		star->AddPlanet(planet);
	}

	return star;
}

Resource SpaceThread::CreateResource(ResourceType type, PlanetType planet, PlanetEnvironment env)
{
	float base = m_planet_resources[(int)type][(int)planet];
	base *= (1 + m_resource_normal_dist(m_generator) / 100);

	if (planet == PlanetType::TERRESTRIAL)
	{
		float mod = m_resource_modifiers[(int)type][(size_t)env - 1];
		mod *= (1 + m_resource_normal_dist(m_generator) / 100);
		base *= (1 + mod / 100);
	}

	base = (base < 0) ? 0 : base;

	float recharge_rate = 0;
	if (base > 0)
	{
		recharge_rate = m_resource_recharge_rates[(int)type][(int)env];
		if (recharge_rate > 0)
		{
			recharge_rate *= (1 + m_recharge_normal_dist(m_generator) / 100);

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
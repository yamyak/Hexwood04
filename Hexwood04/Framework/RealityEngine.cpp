#include "RealityEngine.h"
#include "../Utilities/ConfigReader.h"

#include <thread>


using namespace Constants;

static void Create_Thread(int seed, Universe& verse, StarTable& starDB, int start, int end)
{
	RealityEngine engine(seed);

	bool status = engine.CreateUniverse(verse, starDB, start, end);
}

void Create(int thread_count, StarTable& stars, Universe& verse)
{
	int star_count = stars.GetSize();
	int sector = star_count / thread_count;

	std::vector<std::thread> threads;
	for (int i = 0; i < thread_count; i++)
	{
		int start = i * sector;
		int end = (i + 1) * sector;
		if (i == thread_count - 1)
		{
			end += star_count % thread_count;
		}

		threads.push_back(std::thread(Create_Thread, rand(), std::ref(verse), std::ref(stars), start, end));
	}

	for (auto& thr : threads)
	{
		thr.join();
	}
}

RealityEngine::RealityEngine(int seed)
{
	m_max_planets = ConfigReader::GetInstance()->GetInt(Constants::SOLAR_SYSTEM, Constants::MAX_PLANETS);
	m_gas_giant_min_size = ConfigReader::GetInstance()->GetFloat(Constants::SOLAR_SYSTEM, Constants::GAS_GIANT_MIN_SIZE);
	m_gas_giant_max_size = ConfigReader::GetInstance()->GetFloat(Constants::SOLAR_SYSTEM, Constants::GAS_GIANT_MAX_SIZE);
	m_terrestrial_min_size = ConfigReader::GetInstance()->GetFloat(Constants::SOLAR_SYSTEM, Constants::TERRESTRIAL_MIN_SIZE);
	m_terrestrial_max_size = ConfigReader::GetInstance()->GetFloat(Constants::SOLAR_SYSTEM, Constants::TERRESTRIAL_MAX_SIZE);
	m_planet_resources = ConfigReader::GetInstance()->GetFloatArray(Constants::SOLAR_SYSTEM, Constants::PLANET_RESOURCES);
	m_resource_modifiers = ConfigReader::GetInstance()->GetFloatArray(Constants::SOLAR_SYSTEM, Constants::RESOURCE_MODIFIERS);
	m_generator.seed(seed);
	m_uniform_dist.param(std::uniform_int_distribution<int>::param_type(0, std::numeric_limits<int>::max()));
	m_normal_dist.param(std::normal_distribution<float>::param_type(0, ConfigReader::GetInstance()->GetFloat(Constants::SOLAR_SYSTEM, Constants::RESOURCE_SIGMA)));
}

RealityEngine::~RealityEngine()
{

}

bool RealityEngine::CreateUniverse(Universe& verse, StarTable& starDB, int start, int end)
{
	for (int i = start; i < end; i++)
	{
		StarData data = starDB.GetStar(i);

		Star star = CreateSystem(data);

		verse.AddStar(star);
	}

	return true;
}

Star RealityEngine::CreateSystem(StarData& data)
{
	Star star(data.id, data.name, data.x, data.y, data.z);

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

		Planet planet(i, type, env, resources);
		star.AddPlanet(planet);
	}

	return star;
}

Resource RealityEngine::CreateResource(ResourceType type, PlanetType planet, PlanetEnvironment env)
{
	float base = m_planet_resources[(int)type][(int)planet];
	base *= (1 + m_normal_dist(m_generator) / 100);

	if (planet == PlanetType::TERRESTRIAL)
	{
		float mod = m_resource_modifiers[(int)type][(size_t)env - 1];
		mod *= (1 + m_normal_dist(m_generator) / 100);
		base *= (1 + mod / 100);
	}

	Resource r;
	r.m_type = type;
	r.m_current = (base < 0) ? 0 : base;
	r.m_max = r.m_current;

	return r;
}
#include "RealityEngine.h"
#include "../Constants.h"
#include "../Utilities/ConfigReader.h"
#include "../Reality/Planet.h"

#include <cstdlib>

RealityEngine::RealityEngine(int seed)
{
	srand(seed);
}

RealityEngine::~RealityEngine()
{

}

bool RealityEngine::GenerateUniverse(Universe& verse, StarTable& starDB, int start, int end)
{
	for (int i = start; i < end; i++)
	{
		StarData data = starDB.GetStar(i);

		Star star = GenerateStar(data);

		GenerateSystem(star);

		verse.AddStar(star);
	}

	return true;
}

Star RealityEngine::GenerateStar(StarData& data)
{
	Star star(data.id, data.name, data.x, data.y, data.z);

	return star;
}

void RealityEngine::GenerateSystem(Star& star)
{
	int planet_number = rand() % ConfigReader::GetInstance()->GetInt(Constants::SOLAR_SYSTEM, Constants::MAX_PLANETS);;

	for (int i = 0; i < planet_number; i++)
	{
		PlanetType type = (PlanetType) (rand() % TYPE_MAX);

		PlanetEnvironment env = NONE;
		if (type == TERRESTRIAL)
		{
			env = (PlanetEnvironment)(rand() % (ENVIRONMENT_MAX - 1) + 1);
		}

		Planet planet(i, type, env);
		star.AddPlanet(planet);
	}
}
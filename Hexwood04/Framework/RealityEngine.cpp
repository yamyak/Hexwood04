#include "RealityEngine.h"

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

		verse.AddStar(star);
	}

	return true;
}

Star RealityEngine::GenerateStar(StarData& data)
{
	Star star;

	return star;
}
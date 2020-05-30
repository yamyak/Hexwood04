#pragma once

#include "StarTable.h"
#include "../Reality/Universe.h"
#include "../Reality/Star.h"

class RealityEngine
{
public:
	RealityEngine(int seed);
	~RealityEngine();

	bool GenerateUniverse(Universe& verse, StarTable& starDB, int start, int end);

private:
	Star GenerateStar(StarData& data);
};


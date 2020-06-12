#pragma once

#include "SpaceThread.h"

class SpaceEngine
{
public:
	SpaceEngine();
	~SpaceEngine();

	void Create(int thread_count, StarTable& stars, Universe& verse);
	static void Start_Thread(int seed, Universe& verse, StarTable& starDB, int start, int end);
};


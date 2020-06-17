#pragma once

class StarTable;

class SpaceEngine
{
public:
	SpaceEngine();
	~SpaceEngine();

	void Create(int thread_count, StarTable& stars);
	static void Start_Thread(int seed, StarTable& starDB, int start, int end);
};


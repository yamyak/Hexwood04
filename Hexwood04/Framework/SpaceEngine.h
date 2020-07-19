#pragma once

class StarTable;

/// <summary>
/// Spawns threads that populate subsections of the universe with stars and planets 
/// </summary>
class SpaceEngine
{
public:
	SpaceEngine();
	~SpaceEngine();

	void Create(int thread_count, StarTable& stars);
	static void Start_Thread(int seed, StarTable& starDB, int start, int end);
};


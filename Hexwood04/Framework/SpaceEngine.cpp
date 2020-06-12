#include "SpaceEngine.h"

#include <thread>


SpaceEngine::SpaceEngine()
{

}

SpaceEngine::~SpaceEngine()
{

}

void SpaceEngine::Create(int thread_count, StarTable& stars, Universe& verse)
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

		threads.push_back(std::thread(&SpaceEngine::Start_Thread, rand(), std::ref(verse), std::ref(stars), start, end));
	}

	for (auto& thr : threads)
	{
		thr.join();
	}
}

void SpaceEngine::Start_Thread(int seed, Universe& verse, StarTable& starDB, int start, int end)
{
	SpaceThread space(seed);

	bool status = space.CreateUniverse(verse, starDB, start, end);
}
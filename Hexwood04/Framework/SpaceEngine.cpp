#include "SpaceEngine.h"
#include "SpaceThread.h"

#include <thread>


/// <summary>
/// Constructor
/// </summary>
/// <returns></returns>
SpaceEngine::SpaceEngine()
{

}

/// <summary>
/// Constructor
/// </summary>
/// <returns></returns>
SpaceEngine::~SpaceEngine()
{

}

/// <summary>
/// Starts threads that populate universe and wait for their completion 
/// </summary>
/// <param name="thread_count">Number of threads to run on</param>
/// <param name="stars">Star database</param>
void SpaceEngine::Create(int thread_count, StarTable& stars)
{
	// divide up work based on how many threads and stars there are
	int star_count = stars.GetSize();
	int sector = star_count / thread_count;

	std::vector<std::thread> threads;
	for (int i = 0; i < thread_count; i++)
	{
		// determine which subsection of the universe each thread is responsibe for
		// start and end indices into star database
		int start = i * sector;
		int end = (i + 1) * sector;
		if (i == thread_count - 1)
		{
			end += star_count % thread_count;
		}

		// start thread with random seed value and reference to star database
		threads.push_back(std::thread(&SpaceEngine::Start_Thread, rand(), std::ref(stars), start, end));
	}

	// wait for all threads to finish execution
	for (auto& thr : threads)
	{
		thr.join();
	}
}

/// <summary>
/// Populate a subsection of the universe 
/// Running on a separate thread
/// </summary>
/// <param name="seed">Thread seed value</param>
/// <param name="starDB">Star database</param>
/// <param name="start">Start index into star database for this thread</param>
/// <param name="end">End index into star database for this thread</param>
void SpaceEngine::Start_Thread(int seed, StarTable& starDB, int start, int end)
{
	SpaceThread space(seed);

	bool status = space.CreateUniverse(starDB, start, end);
}
#include "Constants.h"
#include "Utilities/ConfigReader.h"
#include "Framework/StarTable.h"
#include "Framework/RealityEngine.h"
#include "Reality/Universe.h"

#include <thread>
#include <cstdlib>
#include <chrono>

void Creation(int seed, StarTable& starDB, int start, int end, Universe& verse)
{
	RealityEngine engine(seed);

	bool status = engine.GenerateUniverse(verse, starDB, start, end);
}

int main()
{
	std::string config = "E:\\Projects\\Hexwood04\\Config.ini";
	ConfigReader ini_reader;
	ini_reader.ReadConfig(config);

	StarTable stars;
	stars.ReadCSV(*&ini_reader.Get(Constants::INITIALIZATION, Constants::STAR_DATABASE));

	int thread_count = std::thread::hardware_concurrency();
	if (ini_reader.GetBool(Constants::INITIALIZATION, Constants::MANUAL_THREADS))
	{
		thread_count = ini_reader.GetInt(Constants::INITIALIZATION, Constants::THREAD_COUNT);
	}
	thread_count = thread_count == 0 ? 1 : thread_count;

	Universe verse;

	srand(ini_reader.GetInt(Constants::INITIALIZATION, Constants::SEED));

	int star_count = stars.GetSize();
	int sector = star_count / thread_count;

	auto t_start = std::chrono::high_resolution_clock::now();

	std::vector<std::thread> threads;
	for (int i = 0; i < thread_count; i++)
	{
		int seed = rand() % 1000;
		int start = i * sector;
		int end = (i + 1) * sector;
		if (i == thread_count - 1)
		{
			end += star_count % thread_count;
		}

		threads.push_back(std::thread(Creation, seed, std::ref(stars), start, end, std::ref(verse)));
	}

	for (auto& thr : threads)
	{
		thr.join();
	}

	return 0;
}
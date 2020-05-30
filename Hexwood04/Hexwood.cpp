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

void UniverseCreation(int thread_count, StarTable& stars, Universe& verse)
{
	int star_count = stars.GetSize();
	int sector = star_count / thread_count;

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
}

int main()
{
	std::string config = "E:\\Projects\\Hexwood04\\Config.ini";
	ConfigReader::GetInstance()->ReadConfig(config);

	StarTable stars;
	stars.ReadCSV(*&ConfigReader::GetInstance()->Get(Constants::INITIALIZATION, Constants::STAR_DATABASE));

	int thread_count = std::thread::hardware_concurrency();
	if (ConfigReader::GetInstance()->GetBool(Constants::INITIALIZATION, Constants::MANUAL_THREADS))
	{
		thread_count = ConfigReader::GetInstance()->GetInt(Constants::INITIALIZATION, Constants::THREAD_COUNT);
	}
	thread_count = thread_count == 0 ? 1 : thread_count;

	Universe verse;

	srand(ConfigReader::GetInstance()->GetInt(Constants::INITIALIZATION, Constants::SEED));

	UniverseCreation(thread_count, stars, verse);

	return 0;
}
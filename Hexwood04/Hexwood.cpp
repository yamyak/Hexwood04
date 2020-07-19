#include "Constants.h"
#include "Utilities/ConfigReader.h"
#include "Framework/StarTable.h"
#include "Framework/SpaceEngine.h"
#include "Framework/LifeEngine.h"
#include "Framework/TimeEngine.h"
#include "Reality/Universe.h"

/// <summary>
/// Entry point into simulation
/// </summary>
/// <returns></returns>
int main()
{
	// read in and parse configuration file
	std::string config = "E:\\Projects\\Hexwood04\\Config.ini";
	ConfigReader::GetInstance()->ReadConfig(config);

	// retrieve input csv file name from config file
	// read in the star database from a csv file
	StarTable stars;
	stars.ReadCSV(*&ConfigReader::GetInstance()->Get(Constants::INITIALIZATION, Constants::STAR_DATABASE));

	// determine # of threads to run simulation on
	// read from configuration file if set manually
	// other wise use the number of cores available
	int thread_count = std::thread::hardware_concurrency();
	if (ConfigReader::GetInstance()->GetBool(Constants::INITIALIZATION, Constants::MANUAL_THREADS))
	{
		thread_count = ConfigReader::GetInstance()->GetInt(Constants::INITIALIZATION, Constants::THREAD_COUNT);
	}
	thread_count = thread_count == 0 ? 1 : thread_count;

	// initialize the overall RNG with seed form config file
	srand(ConfigReader::GetInstance()->GetInt(Constants::INITIALIZATION, Constants::SEED));

	// initialize the simulation space (the universe)
	Universe::GetInstance();

	// populate the universe with stars and planets
	SpaceEngine spaceEngine;
	spaceEngine.Create(thread_count, stars);

	// populate the universe with early life
	LifeEngine lifeEngine(rand());
	lifeEngine.CreateEmpires();

	// run the simulation
	TimeEngine timeEngine;
	timeEngine.Start(thread_count);

	return 0;
}
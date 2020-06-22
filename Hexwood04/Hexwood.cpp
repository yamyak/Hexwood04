#include "Constants.h"
#include "Utilities/ConfigReader.h"
#include "Framework/StarTable.h"
#include "Framework/SpaceEngine.h"
#include "Framework/LifeEngine.h"
#include "Framework/TimeEngine.h"
#include "Reality/Universe.h"


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

	thread_count = 1;

	srand(ConfigReader::GetInstance()->GetInt(Constants::INITIALIZATION, Constants::SEED));

	Universe::GetInstance();

	SpaceEngine spaceEngine;
	spaceEngine.Create(thread_count, stars);

	LifeEngine lifeEngine(rand());
	lifeEngine.CreateEmpires();

	TimeEngine timeEngine;
	timeEngine.Start(thread_count);

	return 0;
}
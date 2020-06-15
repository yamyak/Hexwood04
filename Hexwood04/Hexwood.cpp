#include "Constants.h"
#include "Utilities/ConfigReader.h"
#include "Framework/StarTable.h"
#include "Framework/SpaceEngine.h"
#include "Framework/LifeEngine.h"
#include "Framework/TimeEngine.h"
#include "Reality/Universe.h"

#include <chrono>
#include <iostream>
#include <fstream>


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

	srand(ConfigReader::GetInstance()->GetInt(Constants::INITIALIZATION, Constants::SEED));

	Universe verse;

	auto start = std::chrono::high_resolution_clock::now();

	SpaceEngine spaceEngine;
	spaceEngine.Create(thread_count, stars, verse);

	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

	std::ofstream myfile("example.txt");
	if (myfile.is_open())
	{
		myfile << duration.count() << std::endl;
		myfile.close();
	}

	LifeEngine lifeEngine(rand());
	lifeEngine.CreateEmpires(verse);

	TimeEngine timeEngine(static_cast<Object*>(&verse));
	timeEngine.Start(thread_count);

	return 0;
}
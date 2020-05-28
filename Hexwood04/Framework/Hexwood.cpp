#include "StarTable.h"

#include "ConfigReader.h"
#include "StarTable.h"
#include "../Constants.h"

int main()
{
	std::string config = "E:\\Projects\\Hexwood04\\Config.ini";
	ConfigReader ini_reader;
	ini_reader.ReadConfig(config);

	StarTable stars;
	stars.ReadCSV(*&ini_reader.Get(Constants::INITIALIZATION, Constants::STAR_DATABASE));
}
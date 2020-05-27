#pragma once

#include <string>
#include <map>

class ConfigReader
{
public:
	ConfigReader(std::string & path);
	~ConfigReader();

	std::string Get(std::string & section, std::string & key);
	int GetInt(std::string & section, std::string & key);
	float GetFloat(std::string & section, std::string & key);

private:
	std::map<std::string, std::map<std::string, std::string>> m_config_map;

	bool ReadCSV(std::string & path);
};


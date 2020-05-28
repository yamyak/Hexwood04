#pragma once

#include <string>
#include <map>

class ConfigReader
{
public:
	ConfigReader();
	~ConfigReader();

	bool ReadConfig(std::string& path);

	std::string Get(const std::string& section, const std::string& key);
	int GetInt(const std::string& section, const std::string& key);
	float GetFloat(const std::string& section, const std::string& key);

private:
	std::map<std::string, std::map<std::string, std::string>> m_config_map;
};


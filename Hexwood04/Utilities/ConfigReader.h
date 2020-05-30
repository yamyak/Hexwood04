#pragma once

#include <string>
#include <map>

class ConfigReader
{
public:
	~ConfigReader();

	static ConfigReader* GetInstance();

	bool ReadConfig(std::string& path);

	std::string Get(const std::string& section, const std::string& key);
	int GetInt(const std::string& section, const std::string& key);
	float GetFloat(const std::string& section, const std::string& key);
	bool GetBool(const std::string& section, const std::string& key);

private:
	static ConfigReader* m_instance;
	std::map<std::string, std::map<std::string, std::string>> m_config_map;

	ConfigReader();
};


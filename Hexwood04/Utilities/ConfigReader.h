#pragma once

#include <string>
#include <map>
#include <vector>



/// <summary>
/// Reads in and parses the INI file provided
/// Singleton class 
/// </summary>
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
	std::vector<std::vector<float>> GetFloatArray(const std::string& section, const std::string& key);
	std::vector<std::vector<int>> GetIntArray(const std::string& section, const std::string& key);

private:
	static ConfigReader* m_instance;

	// dictionary of key value pairs from config file, separated into diffrent sections
	std::map<std::string, std::map<std::string, std::string>> m_config_map;

	ConfigReader();
};


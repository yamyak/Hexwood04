#include "ConfigReader.h"

#include <regex>
#include <fstream>

ConfigReader::ConfigReader(std::string & path)
{
	bool status = ReadCSV(path);
}

ConfigReader::~ConfigReader()
{

}

bool ConfigReader::ReadCSV(std::string & path)
{
	std::string line;
	std::ifstream myfile(path);
  if (myfile.is_open())
  {
    bool first = true;
    std::string section;
    std::map<std::string, std::string> section_map;
    while (getline(myfile, line))
    {
      if (std::regex_match(line.begin(), line.end(), std::regex("^\\[\\w+\\]$")))
      {
        if (!first)
        {
          m_config_map[section] = section_map;
        }

        section = line.substr(1, line.size() - 2);
        section_map.clear();
        first = false;
      }
      else if(std::regex_match(line.begin(), line.end(), std::regex(".+=.+")))
      {
        std::string first = line.substr(0, line.find("="));
        first.erase(std::remove(first.begin(), first.end(), ' '), first.end());

        std::string second = line.substr(line.find("=") + 1);
        second.erase(std::remove(second.begin(), second.end(), ' '), second.end());

        section_map[first] = second;
      }
    }
    m_config_map[section] = section_map;

    myfile.close();
  }

  return true;
}

std::string ConfigReader::Get(std::string & section, std::string & key)
{
  if (m_config_map.count(section) > 0 && m_config_map[section].count(key) > 0)
  {
    return m_config_map[section][key];
  }

  return "";
}

int ConfigReader::GetInt(std::string & section, std::string & key)
{
  return std::stoi(Get(section, key));
}

float ConfigReader::GetFloat(std::string& section, std::string& key)
{
  return std::stof(Get(section, key));
}
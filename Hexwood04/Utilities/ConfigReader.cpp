#include "ConfigReader.h"

#include <regex>
#include <fstream>
#include <sstream>


ConfigReader* ConfigReader::m_instance = nullptr;

ConfigReader::ConfigReader()
{
}

ConfigReader::~ConfigReader()
{
  delete m_instance;
  m_instance = nullptr;
}

ConfigReader* ConfigReader::GetInstance()
{
  if (m_instance == nullptr)
  {
    m_instance = new ConfigReader();
  }

  return m_instance;
}

bool ConfigReader::ReadConfig(std::string& path)
{
	std::string line;
	std::ifstream myfile(path);
  if (myfile.is_open())
  {
    bool first = true;
    bool array = false;
    std::string section;
    std::string array_name;
    std::string array_line;
    std::map<std::string, std::string> section_map;
    while (getline(myfile, line))
    {
      if (std::regex_match(line.begin(), line.end(), std::regex("^#.*")))
      {
        continue;
      }
      else if (std::regex_match(line.begin(), line.end(), std::regex("^\\[\\w+\\]$")))
      {
        if (!first)
        {
          m_config_map[section] = section_map;
        }

        section = line.substr(1, line.size() - 2);
        section_map.clear();
        first = false;
      }
      else if (std::regex_match(line.begin(), line.end(), std::regex("^\\{\\w+\\}$")))
      {
        if (array)
        {
          array_name = line.substr(1, line.size() - 2);
          section_map[array_name] = array_line.substr(0, array_line.size()-1);
        }
        else
        {
          array_line = "";
        }
        array = !array;
      }
      else if(std::regex_match(line.begin(), line.end(), std::regex(".+=.+")))
      {
        std::string first = line.substr(0, line.find("="));
        first.erase(std::remove(first.begin(), first.end(), ' '), first.end());

        std::string second = line.substr(line.find("=") + 1);
        second.erase(std::remove(second.begin(), second.end(), ' '), second.end());

        section_map[first] = second;
      }
      else if (array)
      {
        array_line = array_line + line + ";";
      }
    }
    m_config_map[section] = section_map;

    myfile.close();
  }

  return true;
}

std::string ConfigReader::Get(const std::string& section, const std::string& key)
{
  if (m_config_map.count(section) > 0 && m_config_map[section].count(key) > 0)
  {
    return m_config_map[section][key];
  }

  return "";
}

int ConfigReader::GetInt(const std::string& section, const std::string& key)
{
  return std::stoi(Get(section, key));
}

float ConfigReader::GetFloat(const std::string& section, const std::string& key)
{
  return std::stof(Get(section, key));
}

bool ConfigReader::GetBool(const std::string& section, const std::string& key)
{
  return std::stoi(Get(section, key)) == 0 ? false : true;
}

std::vector<std::vector<float>> ConfigReader::GetFloatArray(const std::string& section, const std::string& key)
{
  std::vector<std::vector<float>> output;
  std::string line = Get(section, key);

  std::stringstream ss;
  ss.str(line);

  std::vector<std::string> tokens;

  std::string temp;
  while (getline(ss, temp, ';'))
  {
    tokens.push_back(temp);
  }

  for (std::string& token : tokens)
  {
    std::stringstream ss2;
    ss2.str(token);

    std::vector<float> values;

    std::string temp2;
    while (getline(ss2, temp2, ' '))
    {
      if (!temp2.empty())
      {
        values.push_back(std::stof(temp2));
      }
    }

    output.push_back(values);
  }

  return output;
}
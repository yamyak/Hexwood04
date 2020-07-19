#include "ConfigReader.h"

#include <regex>
#include <fstream>
#include <sstream>


ConfigReader* ConfigReader::m_instance = nullptr;

/// <summary>
/// Constructor
/// </summary>
/// <returns></returns>
ConfigReader::ConfigReader()
{
}

/// <summary>
/// Destructor
/// </summary>
/// <returns></returns>
ConfigReader::~ConfigReader()
{
  delete m_instance;
  m_instance = nullptr;
}

/// <summary>
/// Singleton accessor
/// </summary>
/// <returns>Pointer to ConfigReader singleton</returns>
ConfigReader* ConfigReader::GetInstance()
{
  if (m_instance == nullptr)
  {
    m_instance = new ConfigReader();
  }

  return m_instance;
}

/// <summary>
/// Opens and reads the INI file at path provided 
/// </summary>
/// <param name="path">INI file path</param>
/// <returns>Whether INI file was read without errors</returns>
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
      // move on to next if current line is a comment
      if (std::regex_match(line.begin(), line.end(), std::regex("^#.*")))
      {
        continue;
      }
      // if line has square brackets: "[...]"
      else if (std::regex_match(line.begin(), line.end(), std::regex("^\\[\\w+\\]$")))
      {
        // if not first section, save the previous section into the configuration map
        if (!first)
        {
          m_config_map[section] = section_map;
        }

        // retrieve section name from inside square brackets
        // reset the section map in preparation for being populated
        section = line.substr(1, line.size() - 2);
        section_map.clear();
        first = false;
      }
      // if line has curly braces: "{...}"
      else if (std::regex_match(line.begin(), line.end(), std::regex("^\\{\\w+\\}$")))
      {
        // if curly braces at end of table, retrieve array name from inside curly braces
        // add array to section map as 1 long string of values
        if (array)
        {
          array_name = line.substr(1, line.size() - 2);
          section_map[array_name] = array_line.substr(0, array_line.size()-1);
        }
        // if curly braces at beginning of table, reset the array string
        // set array flag to signal that parsing array now
        else
        {
          array_line = "";
        }
        array = !array;
      }
      // if line is key value pair (has an equal sign)
      // retrieve the key and the value individually
      // add pair to section map database
      else if(std::regex_match(line.begin(), line.end(), std::regex(".+=.+")))
      {
        std::string first = line.substr(0, line.find("="));
        first.erase(std::remove(first.begin(), first.end(), ' '), first.end());

        std::string second = line.substr(line.find("=") + 1);
        second.erase(std::remove(second.begin(), second.end(), ' '), second.end());

        section_map[first] = second;
      }
      // if array flag is set, add each line of table to array line
      else if (array)
      {
        array_line = array_line + line + ";";
      }
    }
    // add last section to config map
    m_config_map[section] = section_map;

    myfile.close();
  }

  return true;
}

/// <summary>
/// Retrieve the string value associated with section and key provided
/// Returns an empty string if section and/or is invalid
/// </summary>
/// <param name="section">String key for config file section</param>
/// <param name="key">String key for config file value</param>
/// <returns>String value</returns>
std::string ConfigReader::Get(const std::string& section, const std::string& key)
{
  if (m_config_map.count(section) > 0 && m_config_map[section].count(key) > 0)
  {
    return m_config_map[section][key];
  }

  return "";
}

/// <summary>
/// Retrieves the integer value associated with section and key provided
/// </summary>
/// <param name="section">String key for config file section</param>
/// <param name="key">String key for config file value</param>
/// <returns>Integer value</returns>
int ConfigReader::GetInt(const std::string& section, const std::string& key)
{
  return std::stoi(Get(section, key));
}

/// <summary>
/// Retrieves the float value associated with section and key provided
/// </summary>
/// <param name="section">String key for config file section</param>
/// <param name="key">String key for config file value</param>
/// <returns>Float value</returns>
float ConfigReader::GetFloat(const std::string& section, const std::string& key)
{
  return std::stof(Get(section, key));
}

/// <summary>
/// Retrieves the boolean value associated with section and key provided
/// </summary>
/// <param name="section">String key for config file section</param>
/// <param name="key">String key for config file value</param>
/// <returns>Boolean value</returns>
bool ConfigReader::GetBool(const std::string& section, const std::string& key)
{
  return std::stoi(Get(section, key)) == 0 ? false : true;
}

/// <summary>
/// Retrieves a 2D matrix of float values associated with section and key provided 
/// </summary>
/// <param name="section">String key for config file section</param>
/// <param name="key">String key for config file table</param>
/// <returns>2D array for float values</returns>
std::vector<std::vector<float>> ConfigReader::GetFloatArray(const std::string& section, const std::string& key)
{
  std::vector<std::vector<float>> output;
  std::string line = Get(section, key);

  std::stringstream ss;
  ss.str(line);

  std::vector<std::string> tokens;

  // tokenize the 2D matrix into its rows based of semi-colon
  std::string temp;
  while (getline(ss, temp, ';'))
  {
    tokens.push_back(temp);
  }

  // iterate through rows of matrix
  for (std::string& token : tokens)
  {
    std::stringstream ss2;
    ss2.str(token);

    std::vector<float> values;

    // tokenize the current row into values
    std::string temp2;
    while (getline(ss2, temp2, ' '))
    {
      if (!temp2.empty())
      {
        // convert the value string into a float
        values.push_back(std::stof(temp2));
      }
    }

    output.push_back(values);
  }

  return output;
}

/// <summary>
/// Retrieves a 2D matrix of integer values associated with section and key provided 
/// </summary>
/// <param name="section">String key for config file section</param>
/// <param name="key">String key for config file table</param>
/// <returns>2D array for integer values</returns>
std::vector<std::vector<int>> ConfigReader::GetIntArray(const std::string& section, const std::string& key)
{
  std::vector<std::vector<int>> output;
  std::string line = Get(section, key);

  std::stringstream ss;
  ss.str(line);

  std::vector<std::string> tokens;

  // tokenize the 2D matrix into its rows based of semi-colon
  std::string temp;
  while (getline(ss, temp, ';'))
  {
    tokens.push_back(temp);
  }

  // iterate through rows of matrix
  for (std::string& token : tokens)
  {
    std::stringstream ss2;
    ss2.str(token);

    std::vector<int> values;

    // tokenize the current row into values
    std::string temp2;
    while (getline(ss2, temp2, ' '))
    {
      if (!temp2.empty())
      {
        // convert the value string into an integer
        values.push_back(std::stoi(temp2));
      }
    }

    output.push_back(values);
  }

  return output;
}
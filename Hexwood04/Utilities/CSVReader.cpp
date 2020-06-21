#include "CSVReader.h"

#include <fstream>
#include <sstream>


CSVReader::CSVReader()
{
}

CSVReader::~CSVReader()
{
}

std::vector<std::vector<std::string>> CSVReader::ReadCSV(std::string& path, bool header, std::set<std::string>& columns)
{
	std::vector<std::vector<std::string>> requested_data;

	std::string line;
	std::ifstream myfile(path);
	if (myfile.is_open())
	{
		getline(myfile, line);

		std::set<int> indices = ReadHeader(line, columns);

		//while (getline(myfile, line))
		for(int i = 0; i < 1000; i++)
		{
			getline(myfile, line);
			std::vector<std::string> tokens = ReadLine(line, indices);
			requested_data.push_back(tokens);
		}

		myfile.close();
	}

	return requested_data;
}

std::set<int> CSVReader::ReadHeader(std::string& line, std::set<std::string>& columns)
{
	std::stringstream ss;
	ss.str(line);

	std::set<int> indices;

	int index = 0;
	std::string temp;
	while (getline(ss, temp, ','))
	{
		if (columns.find(temp) != columns.end())
		{
			indices.insert(index);
		}

		index++;
	}
	
	return indices;
}

std::vector<std::string> CSVReader::ReadLine(std::string& line, std::set<int>& indices)
{
	std::stringstream ss;
	ss.str(line);

	std::vector<std::string> tokens;

	int index = 0;
	std::string temp;
	while (getline(ss, temp, ','))
	{
		if (indices.find(index) != indices.end())
		{
			tokens.push_back(temp);
		}

		index++;
	}

	return tokens;
}
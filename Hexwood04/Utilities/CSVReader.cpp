#include "CSVReader.h"

#include <fstream>
#include <sstream>


/// <summary>
/// Constructor
/// </summary>
/// <returns></returns>
CSVReader::CSVReader()
{
}

/// <summary>
/// Destructor
/// </summary>
/// <returns></returns>
CSVReader::~CSVReader()
{
}

/// <summary>
/// Reads in the CSV file provided
/// Only reads in the columns specified by their header values 
/// </summary>
/// <param name="path">CSV file path</param>
/// <param name="header">Whether csv file has header or not</param>
/// <param name="columns">Set of header values for columns that will be recorded for CSV</param>
/// <returns>2D array of CSV file string values</returns>
std::vector<std::vector<std::string>> CSVReader::ReadCSV(std::string& path, bool header, std::set<std::string>& columns)
{
	std::vector<std::vector<std::string>> requested_data;

	std::string line;
	std::ifstream myfile(path);
	if (myfile.is_open())
	{
		getline(myfile, line);

		// read in the csv file header line
		std::set<int> indices = ReadHeader(line, columns);

		while (getline(myfile, line))
		{
			// tokenize line in file
			// returns only the data in columns whose indices were specified
			std::vector<std::string> tokens = ReadLine(line, indices);
			requested_data.push_back(tokens);
		}

		myfile.close();
	}

	return requested_data;
}

/// <summary>
/// Reads in the CSV file header
/// Returns set of columns indices from which data that will be collected 
/// </summary>
/// <param name="line">String line parsed from CSV file</param>
/// <param name="columns">Set of header values for columns that will be recorded for CSV</param>
/// <returns>Set of columns indices for requested columns</returns>
std::set<int> CSVReader::ReadHeader(std::string& line, std::set<std::string>& columns)
{
	std::stringstream ss;
	ss.str(line);

	std::set<int> indices;

	int index = 0;
	std::string temp;
	// tokenize the line
	while (getline(ss, temp, ','))
	{
		// checkf if current columns header name is requested
		if (columns.find(temp) != columns.end())
		{
			// if requested, record the column index
			indices.insert(index);
		}

		index++;
	}
	
	return indices;
}

/// <summary>
/// Reads and parses data line of CSV file 
/// </summary>
/// <param name="line">String line parsed from CSV file</param>
/// <param name="indices">Set of columns indices for requested columns</param>
/// <returns>List of string values from parsed line</returns>
std::vector<std::string> CSVReader::ReadLine(std::string& line, std::set<int>& indices)
{
	std::stringstream ss;
	ss.str(line);

	std::vector<std::string> tokens;

	int index = 0;
	std::string temp;
	// tokenize the line
	while (getline(ss, temp, ','))
	{
		// check whether current index is requested
		if (indices.find(index) != indices.end())
		{
			// if requested, save the data
			tokens.push_back(temp);
		}

		index++;
	}

	return tokens;
}
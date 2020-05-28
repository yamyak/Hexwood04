#pragma once

#include "StarTable.h"

#include <set>

class CSVReader
{
public:
	CSVReader();
	~CSVReader();

	std::vector<std::vector<std::string>> ReadCSV(std::string& path, bool header, std::set<std::string>& columns);

private:
	std::set<int> ReadHeader(std::string& line, std::set<std::string>& columns);
	std::vector<std::string> ReadLine(std::string& line, std::set<int>& indices);
};


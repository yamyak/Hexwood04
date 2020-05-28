#pragma once

#include <vector>
#include <string>

struct StarData
{
	int id;
	std::string name;
	double x;
	double y;
	double z;
	double ci;
	double am;
	double dist;
};

class StarTable
{
public:
	StarTable();
	~StarTable();

	bool ReadCSV(std::string& path);

private:
	std::vector<StarData> m_starDB;
};
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
	StarTable(std::string & path);
	~StarTable();

private:
	void ReadCSV(std::string & path);

	std::vector<StarData> m_starDB;
};
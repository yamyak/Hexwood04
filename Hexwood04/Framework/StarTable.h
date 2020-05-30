#pragma once

#include <vector>
#include <string>

struct StarData
{
	int id;
	std::string name;
	float x;
	float y;
	float z;
	float ci;
	float am;
	float dist;
};

class StarTable
{
public:
	StarTable();
	~StarTable();

	bool ReadCSV(std::string& path);
	int GetSize();
	StarData GetStar(int index);

private:
	std::vector<StarData> m_starDB;
};
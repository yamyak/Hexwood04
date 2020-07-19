#pragma once

#include <vector>
#include <string>

/// <summary>
/// Star data structure 
/// </summary>
struct StarData
{
	// unique id
	int id;
	std::string name;
	// x distance from Sol in km
	float x;
	// y distance from Sol in km
	float y;
	// z distance from Sol in km
	float z;
	// color index
	float ci;
	// absolute magnitude
	float am;
	// overall distance from Sol in km
	float dist;
};

/// <summary>
/// Reads in and stores star data from star database csv
/// </summary>
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
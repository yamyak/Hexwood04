#include "StarTable.h"
#include "../Utilities/CSVReader.h"


/// <summary>
/// Constructor
/// </summary>
/// <returns></returns>
StarTable::StarTable()
{

}

/// <summary>
/// Destructor
/// </summary>
/// <returns></returns>
StarTable::~StarTable()
{
	m_starDB.clear();
}

/// <summary>
/// Read in star database from CSV file
/// </summary>
/// <param name="path">Star database CSV file path</param>
/// <returns>Whether star database was read without errors</returns>
bool StarTable::ReadCSV(std::string& path)
{
	// set which columns need to be collected from csv
	std::set<std::string> columns{ "id", "proper", "dist", "absmag", "ci", "x", "y", "z" };

	// read in the csv file
	CSVReader csv_reader;
	std::vector<std::vector<std::string>> data = csv_reader.ReadCSV(path, true, columns);

	m_starDB.reserve(data.size());

	StarData star;
	for (auto& row : data)
	{
		// if no color index value or distance value is greater than max
		// skip this entry
		if (row[4].empty() || std::stof(row[2]) >= 100000)
		{
			continue;
		}

		star.id = std::stoi(row[0]);
		star.name = row[1];
		// convert distances from parsecs to km
		star.dist = std::stof(row[2]) * 3.262f * 9.461e12f;
		star.am = std::stof(row[3]);
		star.ci = std::stof(row[4]);
		// convert distances from parsecs to km
		star.x = std::stof(row[5]) * 3.262f * 9.461e12f;
		star.y = std::stof(row[6]) * 3.262f * 9.461e12f;
		star.z = std::stof(row[7]) * 3.262f * 9.461e12f;

		m_starDB.push_back(star);
	}

	return true;
}

/// <summary>
/// Returns star table size
/// </summary>
/// <returns>Star table size</returns>
int StarTable::GetSize()
{
	return (int)m_starDB.size();
}

/// <summary>
/// Get star data based on star index
/// </summary>
/// <param name="index">Index into star database</param>
/// <returns>Star data</returns>
StarData StarTable::GetStar(int index)
{
	return m_starDB[index];
}
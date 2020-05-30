#include "StarTable.h"
#include "../Utilities/CSVReader.h"

StarTable::StarTable()
{
}

StarTable::~StarTable()
{
	m_starDB.clear();
}

bool StarTable::ReadCSV(std::string& path)
{
	std::set<std::string> columns{ "id", "proper", "dist", "absmag", "ci", "x", "y", "z" };

	CSVReader csv_reader;
	std::vector<std::vector<std::string>> data = csv_reader.ReadCSV(path, true, columns);

	m_starDB.reserve(data.size());

	StarData star;
	for (auto row : data)
	{
		if (row[4].empty() || std::stof(row[2]) > 100000)
		{
			continue;
		}

		star.id = std::stoi(row[0]);
		star.name = row[1];
		star.dist = std::stof(row[2]);
		star.am = std::stof(row[3]);
		star.ci = std::stof(row[4]);
		star.x = std::stof(row[5]);
		star.y = std::stof(row[6]);
		star.z = std::stof(row[7]);

		m_starDB.push_back(star);
	}

	return true;
}

int StarTable::GetSize()
{
	return (int)m_starDB.size();
}

StarData StarTable::GetStar(int index)
{
	return m_starDB[index];
}
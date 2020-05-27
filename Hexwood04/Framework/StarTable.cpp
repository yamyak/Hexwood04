#include "StarTable.h"

//#include "../csvh/csv.h"

StarTable::StarTable(std::string & path)
{
	ReadCSV(path);
}

StarTable::~StarTable()
{
	m_starDB.clear();
}

void StarTable::ReadCSV(std::string & path)
{
	//io::CSVReader<8> csv_reader(path);
	//csv_reader.read_header(io::ignore_extra_column, "id", "proper", "x", "y", "z", "absmag", "ci", "dist");

	//StarData star;
	//while (csv_reader.read_row(star.id, star.name, star.x, star.y, star.z, star.am, star.ci, star.dist))
	//{
	//	m_starDB.push_back(star);
	//}
}

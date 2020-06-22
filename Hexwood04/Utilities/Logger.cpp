#include "Logger.h"
#include "../Reality/Universe.h"
#include "../Reality/Empire.h"
#include "../Constants.h"


using namespace Constants;

Logger* Logger::m_instance = nullptr;

Logger::Logger()
{

}

Logger::~Logger()
{
	delete m_instance;
	m_instance = nullptr;
}

Logger* Logger::GetInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new Logger();
	}

	return m_instance;
}

bool Logger::OpenFile(std::string file)
{
	m_file.open(file.c_str(), std::iostream::out);

	return m_file.is_open();
}

void Logger::CloseFile()
{
	m_file.close();
}

void Logger::RecordData(long long frame_time)
{
	if (m_file.is_open())
	{
		int age = Universe::GetInstance()->GetAge();
		int empire_count = Universe::GetInstance()->GetEmpireCount();
		int colony_count = Universe::GetInstance()->GetColonyCount();
		int ship_count = Universe::GetInstance()->GetShipCount();

		m_file << age << "," << frame_time << "," << empire_count << "," << colony_count << "," << ship_count << ",";
		std::vector<Object*> empires = Universe::GetInstance()->GetObjects(ObjectType::EMPIRE);
		for (Object* obj : empires)
		{
			Empire* empire = static_cast<Empire*>(obj);

			m_file << empire->GetId() << "," << empire->GetColonies().size() << "," << empire->GetShips().size() << 
				"," << empire->m_potentialEmpireSystems.size() << "," << empire->m_potenialColonySites.size() <<  std::endl;
		}
	}
}
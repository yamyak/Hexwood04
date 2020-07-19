#include "Logger.h"
#include "../Reality/Universe.h"
#include "../Reality/Empire.h"
#include "../Constants.h"


using namespace Constants;

Logger* Logger::m_instance = nullptr;

/// <summary>
/// Constructor
/// </summary>
/// <returns></returns>
Logger::Logger()
{

}

/// <summary>
/// Destructor
/// </summary>
/// <returns></returns>
Logger::~Logger()
{
	delete m_instance;
	m_instance = nullptr;
}

/// <summary>
/// Singleton accessor
/// </summary>
/// <returns>Pointer to Logger singleton</returns>
Logger* Logger::GetInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new Logger();
	}

	return m_instance;
}

/// <summary>
/// Opens the output file
/// </summary>
/// <param name="file">Output file path</param>
/// <returns>Output file opened successfully or not</returns>
bool Logger::OpenFile(std::string file)
{
	m_file.open(file.c_str(), std::iostream::out);

	return m_file.is_open();
}

/// <summary>
/// Closes the output file 
/// </summary>
void Logger::CloseFile()
{
	m_file.close();
}

/// <summary>
/// Log data about the current frame 
/// </summary>
/// <param name="frame_time">Amount of time frame took</param>
void Logger::RecordData(long long frame_time)
{
	if (m_file.is_open())
	{
		int age = Universe::GetInstance()->GetAge();
		int empire_count = Universe::GetInstance()->GetEmpireCount();
		int colony_count = Universe::GetInstance()->GetColonyCount();
		int ship_count = Universe::GetInstance()->GetShipCount();

		// Record general data about current frame in first line
		m_file << age << "," << frame_time << "," << empire_count << "," << colony_count << "," << ship_count << "," << std::endl;
		std::vector<Object*> empires = Universe::GetInstance()->GetObjects(ObjectType::EMPIRE);

		// Iterate through all empires
		for (Object* obj : empires)
		{
			Empire* empire = static_cast<Empire*>(obj);

			// Record empire specific data in current frame in 1 line
			m_file << "," << empire->GetId() << "," << empire->GetColonies().size() << "," << empire->GetShips().size() << 
				"," << empire->m_potentialEmpireSystems.size() << "," << empire->m_potenialColonySites.size() <<  std::endl;
		}
	}
}
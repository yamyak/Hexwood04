#pragma once

#include <iostream>
#include <fstream>

/// <summary>
/// Opens, closes, and writes out data to log file
/// Singleton class
/// </summary>
class Logger
{
public:
	Logger();

	static Logger* GetInstance();
	bool OpenFile(std::string file);
	void CloseFile();
	void RecordData(long long frame_time);

private:
	static Logger* m_instance;
	std::ofstream m_file;

	~Logger();
};


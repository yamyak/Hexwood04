#pragma once

#include <iostream>
#include <fstream>

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


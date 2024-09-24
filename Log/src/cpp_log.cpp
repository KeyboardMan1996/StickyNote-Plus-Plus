#include "cpp_log.h"
#include "LogBuffer.h"
#include <mutex>
#include <fstream>
#include <iostream>
std::shared_ptr<Log::CppLog> Log::CppLog::instance;
Log::CppLog::CppLog()
{
	//初始化
	coutSwitch.reset(new CoutSwitch);
	cerrSwitch.reset(new CerrSwitch);
	//清空log文件信息
	std::fstream file;
	file.open("log.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);
	if (!file.good())
	{
		std::cerr << "log file not init!" << std::endl;
		return;
	}
	file << "==LOG==" << "\n";
	file.close();
}

Log::CppLog::~CppLog()
{

}

std::shared_ptr<Log::CppLog> Log::CppLog::GetInstance()
{
	static std::once_flag flag;
	std::call_once(flag, [&] {
		instance.reset(new CppLog);
		});
	return instance;
}

void Log::CppLog::onCout()
{
	coutSwitch->on();
}

void Log::CppLog::offCout()
{
	coutSwitch->off();
}

void Log::CppLog::onCerr()
{
	cerrSwitch->on();
}

void Log::CppLog::offCerr()
{
	cerrSwitch->off();
}

void Log::CppLog::onAll()
{
	onCout();
	onCerr();
}

void Log::CppLog::offAll()
{
	offCout();
	offCerr();
}



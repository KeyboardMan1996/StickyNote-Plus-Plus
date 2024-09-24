#include "LogBuffer.h"
#include <iostream>
#include <fstream>
#include <time.h>
Log::LogBuffer::LogBuffer()
{
	buffer.reserve(64);
}

/**
* @brief Log::LogBuffer::overflow 将流中的数据放入缓冲区
* @param int c
* @return int
*/
int Log::LogBuffer::overflow(int c /*= EOF*/)
{
	if (c == EOF)
		return c;
	buffer.push_back(c);
}

/**
* @brief Log::LogBuffer::sync 将缓冲区的数据同步到设备
* @return int
*/
int Log::LogBuffer::sync()
{
	//如果缓冲区无数据  则不输出
	if (buffer.empty())
		return 0;

	std::fstream file;
	file.open("log.txt", std::fstream::in | std::fstream::out | std::fstream::app);
	if (!file.good())
	{
		buffer.clear();
		return 0;
	}
	file << getCurrentTime().c_str() << " ==" << getTag().c_str() << "==>> ";
	file << buffer.c_str();
	//如果输出的末尾没有换行，则增加换行符
	auto iter = buffer.rbegin();
	if (*iter != '\n')
		file << '\n';

	buffer.clear();
	file.close();
	return 0;
}

/**
* @brief Log::LogBuffer::getTag 获取输出标签
* @return std::string
*/
std::string Log::LogBuffer::getTag()
{
	return "LOG";
}

/**
* @brief Log::LogBuffer::getCurrentTime 获取当前时间
* @return std::string
*/
std::string Log::LogBuffer::getCurrentTime()
{
	time_t rawtime;
	struct tm* timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, 80, "%Y,%m,%d %H:%M:%S", timeinfo);

	return buffer;
}

Log::CerrSwitch::CerrSwitch()
{
	oBuffer = std::cerr.rdbuf();
	nBuffer = new ErrorBuffer;
}

Log::CerrSwitch::~CerrSwitch()
{
	std::cerr.rdbuf(oBuffer);
	delete nBuffer;
}

void Log::CerrSwitch::on()
{
	std::cerr.rdbuf(nBuffer);
}

void Log::CerrSwitch::off()
{
	std::cerr.rdbuf(oBuffer);
}

Log::CoutSwitch::CoutSwitch()
{
	oBuffer = std::cout.rdbuf();
	nBuffer = new LogBuffer();
}

Log::CoutSwitch::~CoutSwitch()
{
	std::cout.rdbuf(oBuffer);
	delete nBuffer;
}

void Log::CoutSwitch::on()
{
	std::cout.rdbuf(nBuffer);
}

void Log::CoutSwitch::off()
{
	std::cout.rdbuf(oBuffer);
}

Log::ErrorBuffer::ErrorBuffer()
	:isEnd(false)
{

}

int Log::ErrorBuffer::overflow(int c /*= EOF*/)
{
	if (c == '\n')
		isEnd = true;
	return LogBuffer::overflow(c);
}

int Log::ErrorBuffer::sync()
{
	if (!isEnd)
		return 0;
	isEnd = false;
	return LogBuffer::sync();
}

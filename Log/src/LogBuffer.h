#ifndef _LOG_BUFFER_H
#define _LOG_BUFFER_H

#include <streambuf>
namespace Log{
	class  LogBuffer :public std::streambuf{
	public:
		LogBuffer();
		~LogBuffer() = default;
	protected:
		int overflow(int c = EOF) override;
		int sync() override;
		virtual std::string getTag();
	private:
		std::string buffer;
	public:
		std::string getCurrentTime();
	};
	/*
		std::cerr 的缓冲区在同步过程中与std::cout不一致。
		std::cerr 每一个 << 运算符都会触发数据同步函数sync(),并且在这之前，还会先触发cout中的同步函数。
		所以这里在cout中做了处理，如果缓冲区为空，则不向log文件中输出任何内容。
		** 这里对std::cerr 的同步机制做了魔改，莫改为当前输入中有 \n 或者使用了 std::endl，才将数据同步到设备中。
		
	*/
	class ErrorBuffer :public LogBuffer {
	public:
		ErrorBuffer();
		~ErrorBuffer() = default;

	protected:
		int overflow(int c = EOF) override;
		int sync() override;
		std::string getTag() override{
			return "Error";
		};
	private:
		//判断缓冲区中是否有空格
		bool isEnd;
	};

	class Switch {
	public:
		Switch() = default;
		virtual ~Switch() {};
		
		virtual void on() = 0;
		virtual void off() = 0;
	protected:
		std::streambuf* oBuffer, * nBuffer;
	};
	class CerrSwitch:public Switch {
	public:
		CerrSwitch();
		~CerrSwitch();
		void on();
		void off();
	};

	class CoutSwitch :public Switch{
	public:
		CoutSwitch();
		~CoutSwitch();
		void on();
		void off();
	};
}
#endif

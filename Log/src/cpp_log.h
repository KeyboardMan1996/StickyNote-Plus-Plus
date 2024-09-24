#ifndef CPP_LOG_H
#define CPP_LOG_H
#include "LogExport.hpp"
#include <memory>
namespace Log {
	class Switch;
	class LOG_EXPORT CppLog {
	public:
		~CppLog();
		static std::shared_ptr<CppLog> GetInstance();
		//开关输出 每一对函数对应一个std中的标准输出接口
		void onCout();
		void offCout();
		void onCerr();
		void offCerr();

		//关闭所有输出
		void onAll();
		void offAll();
	private:
		CppLog();
	private:
		static std::shared_ptr<CppLog> instance;
		//输出开关
		std::shared_ptr<Switch> coutSwitch, cerrSwitch;
	};

}

#endif

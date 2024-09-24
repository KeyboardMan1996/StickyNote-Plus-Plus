#include "cpp_log.h"
#include <iostream>
int main() {

	auto cpplog = Log::CppLog::GetInstance();
	cpplog->onAll();

	std::cout << "this is cout";
	std::cerr << "ÕâÊÇ´íÎóÊä³ö" << "1" << "2" << "3" << std::endl;

	return 0;
}
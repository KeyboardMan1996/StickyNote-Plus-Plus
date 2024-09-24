#pragma once
#include "LogExport.hpp"
#include <string>
#include <memory>
#include <iostream>

namespace  Log {
	class LOG_EXPORT LogTool {
	public:
		LogTool() = default;
		~LogTool() = default;

	public:
		template <typename PTR>
		static bool ptrIsNull(PTR ptr,const std::string& functionName,const std::string& name);
	};

	template <typename PTR>
	bool Log::LogTool::ptrIsNull(PTR ptr, const std::string& functionName, const std::string& name)
	{
		if (ptr)
			return false;
		std::cerr << functionName << " " << name << " " << "is null!" << std::endl;
		return true;
	}

}

#define PtrIsNull(a) Log::LogTool::ptrIsNull(a,std::string(__FUNCTION__),std::string(#a))
#ifndef __CPP_LOG_EXPORT_HPP
#define __CPP_LOG_EXPORT_HPP

//定义导出宏
#ifdef _CPP_LOG_
#define LOG_EXPORT __declspec(dllexport)
#else
#define LOG_EXPORT   __declspec(dllimport)
#endif 

#endif
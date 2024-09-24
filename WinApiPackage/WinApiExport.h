#ifndef __WIN_API_EXPORT_H__
#define __WIN_API_EXPORT_H__

#ifdef _WIN_API_PACKAGE_
#define WIN_API_EXPORT __declspec(dllexport)
#else
#define WIN_API_EXPORT   __declspec(dllimport)
#endif 

#ifndef _WIN_API_PACKAGE_EXE
#define DATA_VISUALIZATION_EXPORT  
#endif
#endif


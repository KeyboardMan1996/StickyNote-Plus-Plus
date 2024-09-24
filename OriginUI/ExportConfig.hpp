#pragma once
#ifdef _ORIGIN_UI_
#define ORIGIN_UI_EXPORT __declspec(dllexport)
#else
#define ORIGIN_UI_EXPORT   __declspec(dllimport)
#endif 

#ifndef ORIGIN_UI_EXE
#define DATA_VISUALIZATION_EXPORT  
#endif

#ifdef  __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif
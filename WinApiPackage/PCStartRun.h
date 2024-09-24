#ifndef __PC_START_RUN_H__
#define __PC_START_RUN_H__
#include "WinApiExport.h"
#include <windows.h>

WIN_API_EXPORT void AutoStart();
WIN_API_EXPORT void CanclePowerOn();
WIN_API_EXPORT BOOL IsAutoStart();

#endif

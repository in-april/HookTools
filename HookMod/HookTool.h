#pragma once
#include "FuncData.h"

class CHookTool
{
public:
	CHookTool();
	BOOL dispatchProc(PCONTEXT context);
	BOOL SetHookOneByte(FuncItem* func);
	//BOOL SetHookNoTrace(DWORD addr, PHookFunc HookFunc, DWORD retAddr = 0);
};
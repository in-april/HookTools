#include "pch.h"
#include "HookTool.h"
#include "Logger.h"
extern CFuncData g_funcData;
extern CHookTool g_hookTool;

//hook��ת����
LONG WINAPI PvectoredExceptionHandler(PEXCEPTION_POINTERS ExceptionInfo)
{
	DWORD eCode = ExceptionInfo->ExceptionRecord->ExceptionCode;
	PCONTEXT pcont = ExceptionInfo->ContextRecord;
	switch (eCode)
	{
	case EXCEPTION_BREAKPOINT: //int3
	{
		g_hookTool.dispatchProc(pcont);
		return EXCEPTION_CONTINUE_EXECUTION;
		break;
	}
	case EXCEPTION_SINGLE_STEP: //cpuӲ���ϵ�
	{
		if (g_hookTool.dispatchProc(pcont))
		{
			pcont->Dr7 = 0; //ȡ���ϵ�
			pcont->EFlags |= 0x100; //���뵥��ģʽ
		}
		else
		{
			pcont->Dr7 = 0b1010101;
		}
		return EXCEPTION_CONTINUE_EXECUTION;
		break;
	}
	default:
		break;
	}
	return EXCEPTION_CONTINUE_SEARCH;
}
CHookTool::CHookTool()
{
	//CString str;
	//str.Format("dispatchProc addr = 0x%08X", &CHookTool::dispatchProc);
	//AfxMessageBox(str);

	AddVectoredExceptionHandler(1, &PvectoredExceptionHandler);
}


//�ɷ�����
BOOL CHookTool::dispatchProc(PCONTEXT context)
{
	DWORD hookAddr = context->Eip;
	auto it = g_funcData.m_mapFunc.find(hookAddr);
	if (it != g_funcData.m_mapFunc.end())
	{
		FuncItem& data = it->second;
		if (data.HookFunc(&data, context))
		{
			context->Eip = data.codeFixAddr;
		}
		else {
			context->Eip = data.retAddr;
		}
		return TRUE;

	}
	return FALSE;
}

//���ֽ��쳣hook
BOOL CHookTool::SetHookOneByte(FuncItem* func)
{
	DWORD addr = func->address + func->hookOffset;
	BYTE codeLen = func->hookChangeCode;
	DWORD oldProtect;
	DWORD newProtect;
	DWORD codeFixProtext;
	VirtualProtect((LPVOID)addr, codeLen, PAGE_EXECUTE_READWRITE, &oldProtect);

	//����ԭʼ����
	PBYTE pFixCode = new BYTE[codeLen + 5];
	memcpy(pFixCode, (LPVOID)addr, codeLen);
	DWORD jmpCode = addr - (DWORD)pFixCode - 5; //jmpCode ���Ĵ���Ϊ ԭʼ�ֽ� + jmp xxxxxxxx

	pFixCode[codeLen] = 0xE9;
	*(PDWORD)&pFixCode[codeLen + 1] = jmpCode;
	VirtualProtect((LPVOID)pFixCode, codeLen, PAGE_EXECUTE_READWRITE, &codeFixProtext);

	func->codeFixAddr = (DWORD)pFixCode;

	//���öϵ�
	char* code = (char*)addr;
	*code = 0xCC;

	VirtualProtect((LPVOID)addr, sizeof(codeLen), oldProtect, &newProtect);
	return TRUE;
}


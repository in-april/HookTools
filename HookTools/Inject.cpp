#include "pch.h"
#include "Inject.h"

//ע��ĺ���
void __stdcall InjectCode()
{
	unsigned address = 0xCCCCCCCC;
	PREMOTE_DATA p = (PREMOTE_DATA)address;
	//����Ҫע���dll
	p->loadLibrary(p->dllName); 
	unsigned dEntryPoint = p->dEntryPoint;
	//������ڵ�
	__asm
	{
		mov eax, dEntryPoint
		jmp eax
	}
}


int CInject::StartProcess(std::string path)
{
	m_path = path;
	DWORD status = 0;
	STARTUPINFO si{ 0 };
	PROCESS_INFORMATION pi{ 0 };
	si.cb = sizeof(si);
	//����ʽ��������
	BOOL ret = CreateProcessA(
		NULL,
		(LPSTR)path.c_str(),
		NULL,
		NULL,
		FALSE,
		CREATE_SUSPENDED,
		NULL,
		NULL,
		&si,
		&pi);
	if (!ret)
	{
		TRACE("create process error:%d\n", GetLastError());
		return -1;
	}
	m_hProcess = pi.hProcess;
	m_hThread = pi.hThread;
	m_pid = pi.dwProcessId;
    return 0;
}

int CInject::AttachProcess(int pid)
{
	m_pid = pid;
	m_hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    return 0;
}

LPVOID CInject::CreateRemoteData(TCHAR* dllName, DWORD dEntryPoint)
{
	LPVOID remoteAddr = VirtualAllocEx(m_hProcess, 0, 0x3000, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	LPVOID remoteDataAddr = (LPVOID)((unsigned)remoteAddr + 0x2000);

	TRACE("Inject address is 0x%08x\n", remoteAddr);
	if (!remoteAddr) return NULL;

	SIZE_T writeSize = 0;
	REMOTE_DATA remoteData{ 0 };
	remoteData.dEntryPoint = dEntryPoint;
	SetRemoteData(&remoteData, dllName);
	//д��Զ������
	WriteProcessMemory(m_hProcess, remoteDataAddr, &remoteData, 0x1000, &writeSize);


	//����Զ�̴���Ĳ�����ַ
	char _code[0x200]{ 0 };
	memcpy(_code, InjectCode, 0x200);
	for (int i = 0; i < 0x100; i++)
	{
		unsigned* pCode = (unsigned*)(&_code[i]);
		if (pCode[0] == 0xCCCCCCCC)
		{
			pCode[0] = (unsigned)remoteDataAddr;
			break;
		}
	}
	//������д��Ŀ�����
	WriteProcessMemory(m_hProcess, remoteAddr, _code, 0x200, &writeSize);

	return remoteAddr;
}

BOOL CInject::SetRemoteData(PREMOTE_DATA _data, TCHAR* dllName)
{
	HMODULE hKernel = LoadLibrary(_T("kernel32.dll"));
	if (!hKernel) return FALSE;
	_data->loadLibrary = (PLoadLibrary)GetProcAddress(hKernel, "LoadLibraryA");
	//_data->getModuleHandle = (PGetModuleHandle)GetProcAddress(hKernel, "GetModuleHandleA");
	int len = 0;
	while (dllName[len++]);
	memcpy(_data->dllName, dllName, len);
}

int CInject::InjectByOEP(std::string modulePath)
{
	CONTEXT cont;
	cont.ContextFlags = CONTEXT_FULL;
	GetThreadContext(m_hThread, &cont);
	DWORD dEntryPoint = cont.Eax;

	//ע�����ݺʹ���
	LPVOID remoteCodeAddr = CreateRemoteData((char*)modulePath.c_str(), dEntryPoint);

	//�޸���ڵ�
	cont.Eax = (DWORD)remoteCodeAddr;
	if (!SetThreadContext(m_hThread, &cont))
	{
		CString wTxt;
		wTxt.Format("�޸���ڵ�ʧ�ܣ�0x%x", GetLastError());
		AfxMessageBox(wTxt);
	}
	ResumeThread(m_hThread);
	return 0;
}

int CInject::InjectByRemoteThread(std::string modulePath)
{
	return 0;
}

#pragma once
#include <string>

typedef DWORD(WINAPI* PLoadLibrary)(wchar_t* dllName);
typedef DWORD(WINAPI* PGetModuleHandle)(wchar_t* dllName);
//注入的数据格式
typedef struct
{
	wchar_t dllName[0xff];

	//变量
	//入口点
	unsigned dEntryPoint;

	//函数地址
	PLoadLibrary loadLibrary;
	//PGetModuleHandle getModuleHandle;
} *PREMOTE_DATA, REMOTE_DATA;

//注入实现类
class CInject
{
private:
	std::string m_path;
	int m_pid;
	HANDLE m_hProcess;
	HANDLE m_hThread;
public:
	int StartProcess(std::string path);
	int AttachProcess(int pid);
	//分配远程空间内存
	LPVOID CreateRemoteData(TCHAR* dllName, DWORD dEntryPoint);
	//构造远程参数
	BOOL SetRemoteData(PREMOTE_DATA _data, TCHAR* dllName);
	//入口点注入
	int InjectByOEP(std::string modulePath);
	//远程线程注入
	int InjectByRemoteThread(std::string modulePath);
};


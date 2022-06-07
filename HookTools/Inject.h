#pragma once
#include <string>

typedef DWORD(WINAPI* PLoadLibrary)(wchar_t* dllName);
typedef DWORD(WINAPI* PGetModuleHandle)(wchar_t* dllName);
//ע������ݸ�ʽ
typedef struct
{
	wchar_t dllName[0xff];

	//����
	//��ڵ�
	unsigned dEntryPoint;

	//������ַ
	PLoadLibrary loadLibrary;
	//PGetModuleHandle getModuleHandle;
} *PREMOTE_DATA, REMOTE_DATA;

//ע��ʵ����
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
	//����Զ�̿ռ��ڴ�
	LPVOID CreateRemoteData(TCHAR* dllName, DWORD dEntryPoint);
	//����Զ�̲���
	BOOL SetRemoteData(PREMOTE_DATA _data, TCHAR* dllName);
	//��ڵ�ע��
	int InjectByOEP(std::string modulePath);
	//Զ���߳�ע��
	int InjectByRemoteThread(std::string modulePath);
};


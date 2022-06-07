#include "pch.h"
#include "ProcTool.h"
#include "tlhelp32.h"

std::vector<ProcessInfo> CProcTool::GetProcessList()
{
    std::vector<ProcessInfo> ret;
    PROCESSENTRY32 pe32;
    //��ʹ������ṹǰ�����������Ĵ�С
    pe32.dwSize = sizeof(pe32);
    //��ϵͳ�����еĽ����ĸ�����
    HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        printf("CreateToolhelp32Snapshot ����ʧ��.\n");
        return ret;
    }
    //�������̿��գ�������ʾÿ�����̵���Ϣ
    BOOL hasNext = ::Process32First(hProcessSnap, &pe32);
    while (hasNext)
    {
        ProcessInfo info;
        info.pid = pe32.th32ProcessID;
        info.procName = pe32.szExeFile;
        ret.push_back(info);
        hasNext = ::Process32Next(hProcessSnap, &pe32);
    }
    //��Ҫ���������snapshot����
    ::CloseHandle(hProcessSnap);
}

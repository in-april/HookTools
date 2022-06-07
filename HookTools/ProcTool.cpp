#include "pch.h"
#include "ProcTool.h"
#include "tlhelp32.h"

std::vector<ProcessInfo> CProcTool::GetProcessList()
{
    std::vector<ProcessInfo> ret;
    PROCESSENTRY32 pe32;
    //在使用这个结构前，先设置它的大小
    pe32.dwSize = sizeof(pe32);
    //给系统内所有的进程拍个快照
    HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        printf("CreateToolhelp32Snapshot 调用失败.\n");
        return ret;
    }
    //遍历进程快照，轮流显示每个进程的信息
    BOOL hasNext = ::Process32First(hProcessSnap, &pe32);
    while (hasNext)
    {
        ProcessInfo info;
        info.pid = pe32.th32ProcessID;
        info.procName = pe32.szExeFile;
        ret.push_back(info);
        hasNext = ::Process32Next(hProcessSnap, &pe32);
    }
    //不要忘记清除掉snapshot对象
    ::CloseHandle(hProcessSnap);
}

#include "pch.h"
#include "Logger.h"

#ifdef Debug
wchar_t* AnsiToUnicode(const char* szStr)
{
    int nLen = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szStr, -1, NULL, 0);
    if (nLen == 0)
    {
        return NULL;
    }
    wchar_t* pResult = new wchar_t[nLen];
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szStr, -1, pResult, nLen);
    return pResult;
}

//wchar_t*ת转char*
char* UnicodeToAnsi(const wchar_t* szStr)
{
    int nLen = WideCharToMultiByte(CP_ACP, 0, szStr, -1, NULL, 0, NULL, NULL);
    if (nLen == 0)
    {
        return NULL;
    }
    char* pResult = new char[nLen];
    WideCharToMultiByte(CP_ACP, 0, szStr, -1, pResult, nLen, NULL, NULL);
    return pResult;
}

char* getCurrentTime() {
    // 获取当前时间，并规范表示
    char* tmp = new char[64]{ 0 };
    time_t ptime;
    time(&ptime);  // time_t time (time_t* timer);
    strftime(tmp, 64, "%Y-%m-%d %H:%M:%S", localtime(&ptime));
    return tmp;
}

void printLog(CString text)
{
    CStringA output;
    std::ofstream outfile;
    outfile.open(FILE_PATH, std::ios::app);
    output.Format("[%s] %s", getCurrentTime(), text.GetBuffer());
    outfile << output << std::endl;
    outfile.close();
}
#endif
#pragma once
#include <vector>
#include <string>
typedef struct
{
	int pid;
	std::string procName;
	std::string procPath;
} ProcessInfo;
class CProcTool
{
public:
	static std::vector<ProcessInfo> GetProcessList();
};


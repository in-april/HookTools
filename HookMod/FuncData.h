#pragma once
#include "../jsonlib/json.h"
#include <string>
#include <unordered_map>

#define CONFIG_PATH "HookMod.json"

struct FuncItem;
//回调函数
typedef BOOL(*PHookFunc)(FuncItem*, PCONTEXT);

enum class ParamType
{
	Dword, StrA, StrW, Obj, Null
};

struct FuncItem
{
	DWORD address; // 函数首地址
	DWORD hookOffset; //hook点距离函数首地址的偏移
	DWORD hookChangeCode; // hook地址影响的汇编指令的长度（TODO:使用反汇编引擎自动计算）
	PHookFunc HookFunc; //回调函数
	DWORD retAddr; // 根据回调函数的返回值来确定是否要跳回到hook点
	DWORD codeFixAddr; //修复函数的地址
	DWORD paramCount; // 参数个数，默认一个参数占4字节，（TODO:后序导出函数的参数个数自动获取）
	ParamType paramType[10]; //保存参数类型,最多10个参数
	DWORD type; // hook类型
	BOOL enable; // 是否启用
	BOOL isSet; // 是否已经设置Hook

	BOOL isExportFunc; //是否为导出函数，导出函数可以自动获取地址

	// 导出函数
	std::string funcName; // 函数名
	std::string dllName; // 模块名

	// TODO: 非导出函数的匹配
	std::string pattern;
	DWORD offset;
};

class CFuncData
{
public:
	//键值为address + hookOffset，即hook点
	std::unordered_map<int, FuncItem> m_mapFunc; 

private:
	Json::Value m_root;
	std::string m_filePath; //配置文件路径

private:
	int LoadCfgFile();
	int JsonToObj(); // 将json转为对象
	int ObjToJson(); // 将对象转为json
	int Save(); //将新增的配置写回到json文件
public:
	int Load(std::string path = CONFIG_PATH); //加载并解析配置文件
	std::vector<FuncItem*> GetFuncList();
	DWORD GetFuncAddrByName(std::string dllName, std::string funcName); // 根据函数名获取地址
	void SetHookEnable(DWORD key, BOOL status);
	void AddFuncItem(FuncItem& data);
};

//获取HOOK参数函数
BOOL FuncShowParam(FuncItem* func, PCONTEXT context);
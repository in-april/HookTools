#pragma once
#include "../jsonlib/json.h"

#define CONFIG_PATH "./config.json"

typedef struct module
{
	std::string moduleName;
	std::string modulePath;
	//std::string moduleCfgPath;
} Module;

class CSetting
{
private:
	Json::Value m_root;
	std::string m_filePath; //配置文件路径

private:
	int LoadCfgFile();
	int ParserCfg(); // 解析配置文件

public:
	std::vector<std::string> m_InjectType; //注入类型
	std::vector<Module> m_modules; //模块列表

public:
	int Load(std::string path = CONFIG_PATH); //加载并解析配置文件
};


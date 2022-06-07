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
	std::string m_filePath; //�����ļ�·��

private:
	int LoadCfgFile();
	int ParserCfg(); // ���������ļ�

public:
	std::vector<std::string> m_InjectType; //ע������
	std::vector<Module> m_modules; //ģ���б�

public:
	int Load(std::string path = CONFIG_PATH); //���ز����������ļ�
};


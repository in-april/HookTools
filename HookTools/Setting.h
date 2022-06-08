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
	int JsonToObj(); // ��jsonתΪ����
	int ObjToJson(); // ������תΪjson

public:
	//std::vector<std::string> m_InjectType; //ע������
	std::vector<Module> m_modules; //ģ���б�

public:
	int Load(std::string path = CONFIG_PATH); //���ز����������ļ�
	int Save(); //������������д�ص�json�ļ�
	int AddModule(std::string moduleName, std::string modulePath); //���ģ��
	int UpdateModule(int nIndex, std::string moduleName, std::string modulePath);
	int DeleteModule(int nIndex);
};


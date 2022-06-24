#pragma once
#include "../jsonlib/json.h"
#include <string>
#include <unordered_map>

#define CONFIG_PATH "HookMod.json"

struct FuncItem;
//�ص�����
typedef BOOL(*PHookFunc)(FuncItem*, PCONTEXT);

enum class ParamType
{
	Dword, StrA, StrW, Obj, Null
};

struct FuncItem
{
	DWORD address; // �����׵�ַ
	DWORD hookOffset; //hook����뺯���׵�ַ��ƫ��
	DWORD hookChangeCode; // hook��ַӰ��Ļ��ָ��ĳ��ȣ�TODO:ʹ�÷���������Զ����㣩
	PHookFunc HookFunc; //�ص�����
	DWORD retAddr; // ���ݻص������ķ���ֵ��ȷ���Ƿ�Ҫ���ص�hook��
	DWORD codeFixAddr; //�޸������ĵ�ַ
	DWORD paramCount; // ����������Ĭ��һ������ռ4�ֽڣ���TODO:���򵼳������Ĳ��������Զ���ȡ��
	ParamType paramType[10]; //�����������,���10������
	DWORD type; // hook����
	BOOL enable; // �Ƿ�����
	BOOL isSet; // �Ƿ��Ѿ�����Hook

	BOOL isExportFunc; //�Ƿ�Ϊ�����������������������Զ���ȡ��ַ

	// ��������
	std::string funcName; // ������
	std::string dllName; // ģ����

	// TODO: �ǵ���������ƥ��
	std::string pattern;
	DWORD offset;
};

class CFuncData
{
public:
	//��ֵΪaddress + hookOffset����hook��
	std::unordered_map<int, FuncItem> m_mapFunc; 

private:
	Json::Value m_root;
	std::string m_filePath; //�����ļ�·��

private:
	int LoadCfgFile();
	int JsonToObj(); // ��jsonתΪ����
	int ObjToJson(); // ������תΪjson
	int Save(); //������������д�ص�json�ļ�
public:
	int Load(std::string path = CONFIG_PATH); //���ز����������ļ�
	std::vector<FuncItem*> GetFuncList();
	DWORD GetFuncAddrByName(std::string dllName, std::string funcName); // ���ݺ�������ȡ��ַ
	void SetHookEnable(DWORD key, BOOL status);
	void AddFuncItem(FuncItem& data);
};

//��ȡHOOK��������
BOOL FuncShowParam(FuncItem* func, PCONTEXT context);
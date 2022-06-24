#include "pch.h"
#include "FuncData.h"
#include <fstream>
#include "Logger.h"
#include <memory>
#include "DialogMain.h"

extern CDialogMain* g_dlgMain;

int CFuncData::LoadCfgFile()
{
	std::ifstream streamReader(m_filePath, std::ios::binary);
	streamReader.seekg(0, std::ios::end);
	unsigned filesize = streamReader.tellg();
	std::shared_ptr<char> _data(new char[filesize]);
	streamReader.seekg(0, std::ios::beg);
	streamReader.read(_data.get(), filesize);
	streamReader.close();
	Json::Reader reader;
	reader.parse(_data.get(), m_root);
	return 0;
}

int CFuncData::JsonToObj()
{
	m_mapFunc.clear();
	Json::Value funcdata = m_root["FuncList"];
	for (unsigned i = 0; i < funcdata.size(); i++)
	{
		FuncItem tmp;
		tmp.address = funcdata[i]["address"].asInt();
		tmp.hookOffset = funcdata[i]["hookOffset"].asInt();
		tmp.hookChangeCode = funcdata[i]["hookChangeCode"].asInt();
		//tmp.HookFunc = (PHookFunc)funcdata[i]["HookFunc"].asInt();
		tmp.HookFunc = &FuncShowParam;
		tmp.codeFixAddr = funcdata[i]["codeFixAddr"].asInt();
		tmp.paramCount = funcdata[i]["paramCount"].asInt();
		//处理参数类型
		Json::Value types = funcdata[i]["paramType"];
		for (unsigned j = 0; j < types.size(); j++)
		{
			std::string typeStr = types[j].asString();
			if (typeStr.compare("Dword") == 0)
			{
				tmp.paramType[j] = ParamType::Dword;
			}
			else if (typeStr.compare("StrA") == 0)
			{
				tmp.paramType[j] = ParamType::StrA;
			}
			else if (typeStr.compare("StrW") == 0)
			{
				tmp.paramType[j] = ParamType::StrW;
			}
			else if(typeStr.compare("Obj") == 0)
			{
				tmp.paramType[j] = ParamType::Obj;
			}
			else
			{
				tmp.paramType[j] = ParamType::Null;
			}
		}

		tmp.type = funcdata[i]["type"].asInt();
		tmp.enable = funcdata[i]["enable"].asBool();
		tmp.isSet = false;

		tmp.isExportFunc = funcdata[i]["isExportFunc"].asBool();
		if (tmp.isExportFunc)
		{
			// 导出函数
			tmp.funcName = funcdata[i]["funcName"].asString();
			tmp.dllName = funcdata[i]["dllName"].asString();
			tmp.address = GetFuncAddrByName(tmp.dllName, tmp.funcName);
		}
		else
		{
			// 未导出函数
			tmp.pattern = funcdata[i]["pattern"].asString();
			tmp.offset = funcdata[i]["offset"].asInt();
		}
		
		m_mapFunc.insert(std::pair<int, FuncItem>(tmp.address + tmp.hookOffset, tmp));
	}
	return 0;
}

int CFuncData::ObjToJson()
{
    return 0;
}

int CFuncData::Save()
{
	Json::StyledWriter swriter;
	std::string str = swriter.write(m_root);
	std::ofstream streamWriter(m_filePath, std::ios::binary);
	streamWriter.write(str.c_str(), str.size());
	streamWriter.close();
	return 0;
}

int CFuncData::Load(std::string path)
{
	m_filePath = path;
	LoadCfgFile();
	JsonToObj();
	return 0;
}

std::vector<FuncItem*> CFuncData::GetFuncList()
{
	std::vector<FuncItem*> ret;
	auto iter = m_mapFunc.begin();
	for (auto it = m_mapFunc.begin(); it != m_mapFunc.end(); it++) {
		ret.push_back(&it->second);
	}
	return ret;
}

DWORD CFuncData::GetFuncAddrByName(std::string dllName, std::string funcName)
{
	HMODULE hMod = LoadLibraryA(dllName.c_str());
	if (!hMod) return 0;
	DWORD addr = (DWORD)GetProcAddress(hMod, funcName.c_str());
	if (!addr) return 0;
	return addr;
}

void CFuncData::SetHookEnable(DWORD key, BOOL status)
{
	m_mapFunc[key].enable = status;
}

void CFuncData::AddFuncItem(FuncItem& data)
{
	
}

// 函数参数展示回调函数
BOOL FuncShowParam(FuncItem* func, PCONTEXT context)
{
	std::string str;
	if (func->enable)
	{
		str = func->funcName  + "(";
		DWORD paramCount = func->paramCount;
		for (int i = 0; i < paramCount; i++)
		{
			DWORD param = *(DWORD*)(context->Esp + (i + 1) * 4);
			//std::string out = std::to_string((int)func->paramType[i]);
			//::SendMessage(g_dlgMain->m_hWnd, WM_SHOW_DATA, (WPARAM)&out, NULL);
			switch (func->paramType[i])
			{
			case ParamType::Dword:
			{
				str = str + std::to_string(param);
				break;
			}
			case ParamType::StrA:
			{
				char* p = (char*)param;
				str += "\"";
				str += p;
				str += "\"";
				break;
			}
			case ParamType::StrW:
			{
				std::wstring wStr = (wchar_t*)param;
				std::string aStr(wStr.begin(), wStr.end());
				str += "\"";
				str += aStr;
				str += "\"";
				break;
			}
			case ParamType::Obj: // TODO：特殊处理
			{
				str = str + std::to_string(param);
				break;
			}
			default:
			{
				str = str + "NULL";
				break;
			}
			}
			if (i < paramCount - 1) 
				str += ',';
		}
		str += ")\r\n";

		SendMessage(g_dlgMain->m_hWnd, WM_SHOW_DATA, (WPARAM)&str, NULL);
		printLog(str.c_str());
	}
	return TRUE;
}
#include "pch.h"
#include "Setting.h"
#include <fstream>

int CSetting::LoadCfgFile()
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

int CSetting::JsonToObj()
{
	m_modules.clear();
	Json::Value modules = m_root["ModuleList"];
	for (unsigned i = 0; i < modules.size(); i++)
	{
		Module tmp;
		tmp.moduleName = modules[i]["name"].asString();
		tmp.modulePath = modules[i]["path"].asString();
		m_modules.push_back(tmp);
	}
    return 0;
}

int CSetting::ObjToJson()
{
	m_root.clear();
	for (unsigned i = 0; i < m_modules.size(); i++)
	{
		Json::Value moduleTmp;
		moduleTmp["name"] = m_modules[i].moduleName;
		moduleTmp["path"] = m_modules[i].modulePath;
		m_root["ModuleList"].append(moduleTmp);
	}
	Save();
	return 0;
}

int CSetting::Load(std::string path)
{
	m_filePath = path;
	LoadCfgFile();
	JsonToObj();
	return 0;
}

int CSetting::Save()
{
	Json::StyledWriter swriter;
	std::string str = swriter.write(m_root);
	std::ofstream streamWriter(m_filePath, std::ios::binary);
	streamWriter.write(str.c_str(), str.size());
	streamWriter.close();
	return 0;
}

int CSetting::AddModule(std::string moduleName, std::string modulePath)
{
	Module moduleTmp;
	moduleTmp.moduleName = moduleName;
	moduleTmp.modulePath = modulePath;
	m_modules.push_back(moduleTmp);
	ObjToJson();
	return 0;
}

int CSetting::UpdateModule(int nIndex, std::string moduleName, std::string modulePath)
{
	m_modules[nIndex].moduleName = moduleName;
	m_modules[nIndex].modulePath = modulePath;
	ObjToJson();
	return 0;
}

int CSetting::DeleteModule(int nIndex)
{
	m_modules.erase(m_modules.begin() + nIndex);
	ObjToJson();
	return 0;
}

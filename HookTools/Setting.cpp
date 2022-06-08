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

int CSetting::ParserCfg()
{
	//m_InjectType.clear();
	m_modules.clear();
	//Json::Value types = m_root["InjectType"];
	//for (unsigned i = 0; i < types.size(); i++)
	//{
	//	m_InjectType.push_back(types[i].asString());
	//}
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

int CSetting::Load(std::string path)
{
	m_filePath = path;
	LoadCfgFile();
	ParserCfg();
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
	Json::Value moduleTmp;
	moduleTmp["name"] = moduleName;
	moduleTmp["path"] = modulePath;
	Json::Value& modules = m_root["ModuleList"];
	modules.append(moduleTmp);
	ParserCfg();
	Save();
	return 0;
}

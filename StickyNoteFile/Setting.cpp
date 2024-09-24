#include "Setting.h"

NF::Setting::Setting()
{

}

NF::Setting::~Setting()
{
	for (auto pr : propertyList)
	{
		delete pr;
	}
	propertyList.clear();
}

/**
* 从文件中载入数据
* @brief NF::Setting::loadProperty 
* @param const std::string & path 路径，编码为GBK
* @return void
*/
void NF::Setting::loadProperty(const std::string& path)
{
	using namespace pugi;
	xml_document doc;
	xml_parse_result result = doc.load_file(path.c_str());
	if (!result)
		return;
	auto node = doc.child("Property");
	for (auto iter = propertyList.begin(); iter != propertyList.end(); iter++)
	{
		(*iter)->loadValue(node);
	}
}

/**
* 保存文件到数据
* @brief NF::Setting::saveProperty
* @param const std::string & path 路径，编码为GBK
* @return void
*/
void NF::Setting::saveProperty(const std::string& path)
{
	using namespace pugi;
	xml_document doc;
	 doc.load_file(path.c_str());
	 doc.remove_child("Property");
	 auto node = doc.append_child("Property");
	for (auto iter = propertyList.begin(); iter != propertyList.end(); iter++)
	{
		(*iter)->saveValue(node);
	}
	doc.save_file(path.c_str());
}

void NF::Setting::addProperty(Property* p)
{
	propertyList.push_back(p);
}


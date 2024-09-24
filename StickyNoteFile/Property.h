#pragma  once
#include "ExportConfig.hpp"
#include "xml/pugixml.hpp"
#include <string>

/*
* 这个宏用于创建可以存储数据与xml文件的类类型
* PROPERTY_CLASS_HEADER 宏用于创建头文件
* PROPERTY_CLASS_SOURCE 用于创建源文件，这个宏实现再Property.cpp文件中
* 两个宏需要配合使用，否则会编译报错
* type : 要存储的数据类型
* className : 类名的后缀
* 构造对象时需要传入两个参数
* name : 属性名称，这个属性在同一个xml node必须是唯一的，否则在读取时并不能区分。
* value :  默认的值，如果xml attribute 不存在时，读取时会将这个值作为默认值。
*/
#define PROPERTY_CLASS_HEADER(type,className)\
class NOTE_FILE_EXPORT Property##className:public Property{\
public:	\
		Property##className(const std::string& name,const type value);\
		~Property##className() = default;\
		void loadValue(pugi::xml_node& node) override;\
		void saveValue(pugi::xml_node& node) override;\
		type getValue();\
		void setValue(const type& v);\
		void operator =(const type& v);\
private:\
	std::string name;\
	##type value;\
}\
/*
* 快速生成属性变量
*/
#define CREAT_PROPERTY(className,propertyName,propertyValue)\
	className propertyName(#propertyName,propertyValue);


namespace NF {
	class Property {
	public:
		virtual ~Property() {};
		virtual void loadValue(pugi::xml_node& node) =0;
		virtual void saveValue(pugi::xml_node& node) = 0;
	};

	PROPERTY_CLASS_HEADER(int,Int);
	PROPERTY_CLASS_HEADER(double,Double);
	PROPERTY_CLASS_HEADER(float,Float);
	PROPERTY_CLASS_HEADER(unsigned int, UInt);
	PROPERTY_CLASS_HEADER(std::string, String);
	PROPERTY_CLASS_HEADER(bool, Bool);
}
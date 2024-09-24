#include "Property.h"

/*
* type : 要存储的数据类型,用于生成类名
* name : 类名的后缀
* xmlType : 对应xml库中 to_xxx()函数的类型，用于读取
*/
#define PROPERTY_CLASS_SOURCE(type,className,xmlType)\
Property##className::Property##className(const std::string& name,const type v)\
{\
	this->name = name;\
	this->value = v;\
};\
void Property##className::saveValue(pugi::xml_node& node)\
{\
	node.append_attribute(name.c_str()) = value;\
};\
void Property##className::loadValue(pugi::xml_node& node)\
{\
		auto attr = node.attribute(name.c_str());\
	if(attr.empty())\
		return;\
	value = attr.as_##xmlType(value);\
};\
void Property##className::operator =(const  type& v)\
{\
	value = v;\
};\
void Property##className::setValue(const  type& v)\
{\
	value = v;\
};\
type Property##className::getValue()\
{\
	return value;\
};
/*
* PROPERTY_CLASS_SOURCE 的改进型 主要解决类型不能直接用于xml存储，需要做转换的情况下使用
* trName : 转换函数名称，这里其实是针对std::string 转 char 写的
*/
#define PROPERTY_CLASS_SOURCE_TR(type,className,xmlType,trName)\
Property##className::Property##className(const std::string& name,const type v)\
{\
	this->name = name;\
	this->value = v;\
};\
void Property##className::saveValue(pugi::xml_node& node)\
{\
	node.append_attribute(name.c_str()) = value.##trName();\
};\
void Property##className::loadValue(pugi::xml_node& node)\
{\
	auto attr = node.attribute(name.c_str());\
	if(attr.empty())\
		return;\
	value = attr.as_##xmlType(value.##trName());\
};\
void Property##className::operator =(const  type& v)\
{\
	value = v;\
};\
void Property##className::setValue(const  type& v)\
{\
	value = v;\
};\
type Property##className::getValue()\
{\
	return value;\
};

namespace NF {
	PROPERTY_CLASS_SOURCE(int,Int,int);
	PROPERTY_CLASS_SOURCE(float,Float,float);
	PROPERTY_CLASS_SOURCE(double,Double, double);
	PROPERTY_CLASS_SOURCE(unsigned int, UInt, uint);
	PROPERTY_CLASS_SOURCE_TR(std::string, String,string,c_str);
	PROPERTY_CLASS_SOURCE(bool, Bool, bool);
}

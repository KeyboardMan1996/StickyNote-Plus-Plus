#pragma  once
#include <list>
#include "Property.h"
#include "ExportConfig.hpp"
namespace NF {
	class NOTE_FILE_EXPORT Setting {
	public:
		Setting();
		virtual ~Setting();
	public:
		void loadProperty(const std::string& path);
		void saveProperty(const std::string& path);
	
		void addProperty(Property* p);
	private:
		std::list<Property*> propertyList;
	};
}
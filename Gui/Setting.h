#pragma once
#include "StickyNoteFile/Setting.h"
#include <map>
#include <memory>
#include <mutex>
namespace NF{
	class PropertyBool;
	class PropertyString;
}
namespace NGui {
	class Setting :public NF::Setting {
	public:
		
		~Setting();

	public:
		//载入与保存文件数据 使用默认路径
		void loadProperty();
		void saveProperty();
	public:
		//设置默认的配置文件路径
		static void setDefaultConfigPath(const std::string& path);
		static std::string getDefaultConfigPath();
		static bool defaultPathExist();
		static std::shared_ptr<Setting> GetInstance();
	private:
		void initProperty();
		Setting();
		
	
	public:
		//便利贴窗口工具栏是否自动弹出
		NF::PropertyBool* stickyNoteToolbarAutoPop;
		//便利贴窗口是否显示再任务栏
		NF::PropertyBool* stickyNoteToolMod;
		//历史窗口快捷键
		NF::PropertyString* historyWidgetShortcut;
		//忽略版本
		NF::PropertyDouble* ignoreVersion;
		//自动检查更新
		NF::PropertyBool* autoUpdate;
		//用户的GUID，用于记录用户版本
		NF::PropertyString* userID;
		//触发自动吸附的阈值
		NF::PropertyInt* magneticThreshold;
		//是否触发窗口吸附
		NF::PropertyBool* autoMagnetic;
		//窗口吸附后的间距
		NF::PropertyInt* magneticDistance;
		//默认字体
		NF::PropertyString* stickyNoteFont;
		//默认图片存储路径
		NF::PropertyString* imageSavePath;
		//窗口透明
		NF::PropertyBool* transparentWidget;
		//固定到桌面
		NF::PropertyBool* pinIconLayer;
		//默认的便利贴窗口的宽度与高度
		NF::PropertyInt* stickyNoteWidth, * stickyNoteHeight;
		//所有命令的默认状态
		std::map<std::string, NF::PropertyBool*> cmdState;
		//是否自动识别链接
		NF::PropertyBool* visitiUrl;
	private:
		static std::string defaultPath;
		static std::shared_ptr<Setting> _instance;

	};
}
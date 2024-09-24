#pragma  once
#include "Action.h"
#include <memory>
#include <mutex>
#include <map>

namespace NGui {
	class StickyNote;
	class ActionCommand :public OriginUI::Action {
	public:
		virtual void init() = 0;
		virtual void active(StickyNote* note,const bool &check) = 0;
		virtual void updateState(StickyNote* note) {};
		virtual bool isUsable(StickyNote* note) { return true; };
		void setShortcut(const QString& shortcut);
		void setToolTip(const QString& tip);
		QString getShortcut();
	private:
		QString shortcut;
	};
	
	class ActionCommandTypes {
	public:
		ActionCommandTypes(const std::string& cmd,const bool& b);
		std::string cmd;	//命令
		bool visible;		//是否显示
	};

	class ActionCreator {
	public:
		virtual ActionCommand* CreatAction() = 0;
		virtual std::string getCmdName() = 0;
	};

	class ActionCmdFactory {
	public:
		~ActionCmdFactory();
		ActionCmdFactory(const ActionCmdFactory&) = delete;
		ActionCmdFactory operator=(const ActionCmdFactory&) = delete;

		static std::shared_ptr<ActionCmdFactory> GetInstance() {
			std::once_flag flag;
			std::call_once(flag, [&] {
				instance.reset(new ActionCmdFactory());
				instance->registerAllCmd();
				});
			return instance;
		};
	public:
		ActionCommand* creatActionCommand(const std::string& cmd);
	public:
		void registerCmd(ActionCreator * creator);
		void registerAllCmd();
		std::vector<std::string> getAllCmdName();
	private:
		ActionCmdFactory();
		static std::shared_ptr<ActionCmdFactory> instance;
		//构造器集
		std::map<std::string,ActionCreator*> creators;
	};
	
}
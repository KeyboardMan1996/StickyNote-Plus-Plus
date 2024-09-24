#pragma once
#include <vector>
#include <memory>
#include <mutex>
#include "Setting.h"
#include "NoteFile.h"
namespace NF {
	class StickyNote;
}

namespace NGui {
	class StickyNote;
	class ActionCommandStateTimer;
	class StickyNoteMagnetic;
	class StickyNoteCreator;
	class StickyNotePool {
		friend class ActionCommandStateTimer;
	public:
		~StickyNotePool();
		StickyNotePool(const StickyNotePool&) = delete;
		StickyNotePool operator=(const StickyNotePool&) = delete;

		static std::shared_ptr<StickyNotePool> GetInstance() {
			static std::once_flag flag;
			std::call_once(flag, [&]() {
				instance.reset(new StickyNotePool());
				});

			return instance;
		}

	public:
		void StickyNoteCloseActionTirgger(StickyNote* note);
		void closeNoteNoSave(StickyNote* note);
		void closeStickyNote(const std::string& GUID);
		void hideStickyNote(const std::string& GUID);
		StickyNote* getStickyNote(const std::string& GUID);
		void closeAllNote();
		//窗口是否已经被显示
		bool hasFileNote(const std::string& GUID);
		//载入设置
		void loadSetting();
		//注册创建器
		void registerStickyNoteCreator(StickyNoteCreator* creator);
	public:
		static StickyNote* CreatStickyNote(const std::string& type);
		static StickyNote* CreatStickyNote(std::shared_ptr<NF::StickyNote> nfNote);
		static void reloadSetting();
		static void magneticStickyNote(StickyNote* note);
		static void toDeskCenter(std::shared_ptr<NF::StickyNote> note);
	private:
		void initPos(StickyNote* note);
		void remove(StickyNote* note);
	private:
		StickyNotePool();
		static std::shared_ptr<StickyNotePool> instance;
		//记录所有便笺窗口
		std::vector<StickyNote*> pool;
		//存放所有的便笺创建器
		std::map<std::string, StickyNoteCreator*>  stickyNoteCreators;
		//定时刷新便笺窗口的按钮状态
		ActionCommandStateTimer *timer;

		//窗口类型
		bool stickyNoteToolMod;
		//任务栏是否自动弹出
		bool stickyNoteAutoPop;
		//窗口自动吸附触发工具
		StickyNoteMagnetic* magnetic;
	};
}
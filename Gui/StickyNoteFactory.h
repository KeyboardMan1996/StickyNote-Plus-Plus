#pragma  once
#include "StickyNoteFile/NoteFile.h"
#include <memory>
namespace NGui {
	class StickyNote;
	class StickyNoteCreator {
	public:
		/*
		* NGui::StickyNote 需要被便笺池管理，所以不需要智能指针
		*/
		virtual StickyNote* creatStickyNote() = 0;
		virtual StickyNote* creatStickyNote(std::shared_ptr<NF::StickyNote>) = 0;
		//获取类型，用于创建时区分
		virtual std::string getType() = 0;
		//将所有创造器注册到便笺池中
		static void registerAllCreator();

	};
	class TextNoteCreator:public StickyNoteCreator {
	public:
		/*
		* NGui::StickyNote 需要被便笺池管理，所以不需要智能指针
		*/
		virtual StickyNote* creatStickyNote();
		virtual StickyNote* creatStickyNote(std::shared_ptr<NF::StickyNote>);
		//获取类型，用于创建时区分
		virtual std::string getType();

	};

	class ToDoListNoteCreator :public StickyNoteCreator {
	public:
		/*
		* NGui::StickyNote 需要被便笺池管理，所以不需要智能指针
		*/
		virtual StickyNote* creatStickyNote();
		virtual StickyNote* creatStickyNote(std::shared_ptr<NF::StickyNote>);
		//获取类型，用于创建时区分
		virtual std::string getType();

	};

	class DiversityNoteCreator :public StickyNoteCreator {
	public:
		/*
		* NGui::StickyNote 需要被便笺池管理，所以不需要智能指针
		*/
		virtual StickyNote* creatStickyNote();
		virtual StickyNote* creatStickyNote(std::shared_ptr<NF::StickyNote>);
		//获取类型，用于创建时区分
		virtual std::string getType();

	};
}
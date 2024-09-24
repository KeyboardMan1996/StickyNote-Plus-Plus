#pragma once
#include <QEvent>
#include <QString>
#include <QColor>
#include "NoteFile.h"
namespace NGui {
	enum StickyNoteEventType {
		NONE = QEvent::Type::User,
		CONTENT_CHANGE,		//文本改变事件
		COLOR_CHANGE,		//颜色改变
		STICKY_NOTE_CREAT,	//便笺被创建
		STICKY_NOTE_COLSE,	//便笺被关闭
		STICKY_NOTE_DELETE	//便笺删除
	};


	class StickyNoteEvent :public QEvent {
	public:
		StickyNoteEvent(const std::shared_ptr<NF::StickyNote> n,const StickyNoteEventType& type)
			:QEvent((QEvent::Type)type)
		{
			note = n;
		};
		std::shared_ptr<NF::StickyNote> note;

	};

	class StickyNoteContentChangeEvent:public StickyNoteEvent{
	public:
		StickyNoteContentChangeEvent(const std::shared_ptr<NF::StickyNote> n)
		:StickyNoteEvent(n,CONTENT_CHANGE){};
	};

	class StickyNoteColorChangeEvent :public StickyNoteEvent {
	public:
		StickyNoteColorChangeEvent(const std::shared_ptr<NF::StickyNote> n, const QColor& color)
		:StickyNoteEvent(n,COLOR_CHANGE),color(color){};
		QColor color;
	};

	class StickyNoteCreatEvent : public StickyNoteEvent {
	public:
		StickyNoteCreatEvent(const std::shared_ptr<NF::StickyNote> n) 
		:StickyNoteEvent(n, STICKY_NOTE_CREAT){};
	};

	class StickyNoteColseEvent : public StickyNoteEvent {
	public:
		StickyNoteColseEvent(const std::shared_ptr<NF::StickyNote> n) 
		:StickyNoteEvent(n, STICKY_NOTE_COLSE){};
	};

	class StickyNoteDeleteEvent : public StickyNoteEvent {
	public:
		StickyNoteDeleteEvent(const std::shared_ptr<NF::StickyNote> n) 
		:StickyNoteEvent(n, STICKY_NOTE_DELETE) {};
	};
}
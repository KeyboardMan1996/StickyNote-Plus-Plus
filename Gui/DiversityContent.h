#pragma once
#include <QListWidget>
#include <QObject>
#include "StickyNoteContent.h"
#include <qlistwidget.h>
#include <QList>
namespace NGui {
	class DiversityContent:public QListWidget, public StickyNoteContent {
		//Q_OBJECT
	public:
		DiversityContent(QWidget* parent = 0);
		~DiversityContent();
	public:
		//从html中载入内容
		virtual  void loadNote(const std::shared_ptr<NF::StickyNote> note) override;
		//保存内容到html
		virtual  void saveNote(const std::shared_ptr<NF::StickyNote> note) override;
		//链接与便笺窗口的信号和槽
		virtual  void connectStickyNote(StickyNote* stickyNote) override;
		//取消便笺窗口的信号的槽
		virtual	 void disConnectStickyNote(StickyNote* stickyNote) override;
		//判断便笺内容是否为空
		virtual  bool isEmpty() override;
		//初始化页面，可以在里面进行一些对便笺窗口的设置
		virtual  void initGui(StickyNote* stickyNote) override;

	};
};
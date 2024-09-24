#pragma once
#include <QTextEdit>
#include <QWidget>
#include "SimpleRichTextEdit.h"
#include "pugixml.hpp"
#include "StickyNoteContent.h"
class QToolButton;
namespace OriginUI {
	class ToolBar;
}

namespace NGui {
	
	class NoteTextEdit :public OriginUI::SimpleRichTextEdit ,public StickyNoteContent{
		Q_OBJECT
	public:
		NoteTextEdit(QWidget* parent = 0);
		~NoteTextEdit();

	public:
		void initGUI();
		void stopTimer();
		virtual void loadNote(const std::shared_ptr<NF::StickyNote> note) override;
		virtual void connectStickyNote(StickyNote* stickyNote) override;
		virtual void disConnectStickyNote(StickyNote* stickyNote) override;
		virtual void saveNote(const std::shared_ptr<NF::StickyNote> note) override;
		virtual bool isEmpty() override;
		static NoteTextEdit* getNoteTextEdit(StickyNoteContent* content);
	Q_SIGNALS:
		void gotFocus();
		void lostFocus();
		void emitSetHtml(QString);

	protected:
		void focusInEvent(QFocusEvent* e);
		void focusOutEvent(QFocusEvent* e);
		void insertFromMimeData(const QMimeData* source);
		void resizeEvent(QResizeEvent* e);
		void timerEvent(QTimerEvent* e);
	public Q_SLOTS:
		void buttonUpClicked(bool);
		void buttonLowerClicked(bool);
		void slotSetHtml(QString html);
	private:
		//自动设置所有图片的大小
		void autoImageSize(const QString& html,const QSize& widgetSize);
		QSize getImageSize(const QSize& imageSize, const QSize& widgetSize);
		//修改xml中的大小
		void setImageXmlNodeSize(pugi::xml_node node, const QSize& widgetSize);
		void insertImage(const QImage& image);
		void loopXmlNode(pugi::xml_node node, const QSize& widgetSize);
	private:
		int autoImageSizeTimerID;
	};
}
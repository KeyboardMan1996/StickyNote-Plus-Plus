#pragma once
#include <qwidget.h>
#include <qaction.h>
#include <QPoint>
#include <QTextCharFormat>
#include <map>
#include <QVBoxLayout>
#include <memory>
#include <QShortcut>
#include "WidgetNoBoder.h"
#include "NoteFile.h"
#include "ColorWidget.h"
#include "ActionCommand.h"
#include "StickyNoteContent.h"

class QToolButton;
namespace OriginUI {
	class ToolBar;
	class Action;
	class DrawerWidgetTraceMouse;
}
namespace NGui {
	class Pushpin;
	class NoteTextEdit;
	class ColorWidget;
	class StickyNoteD;
	class ActionCommand;
	class StickyNoteSlot;
	class StickyNoteContent;
	class StickyNote : public OriginUI::WidgetNoBoder{
		Q_OBJECT
			friend class StickyNotePool;
	public:
		StickyNote(std::shared_ptr<NF::StickyNote> note, StickyNoteContent* content,QWidget* parent = 0);
		virtual ~StickyNote();
	public:
		void loadNote(std::shared_ptr<NF::StickyNote> nt);
		void saveNote();
		void clearContent();
		std::shared_ptr<NF::StickyNote> getNFNote();
		StickyNoteContent* getContent();

		//弹出颜色设置
		void popColorSetting();
		void setNoteVisable(bool b);
		void connectSlot();
		void disConnectSlot();
		void show();
		void updateCmdState();
		//刷新工具栏 用于刷新工具栏上哪些工具会显示
		void updateToolBar(std::vector<ActionCommandTypes>& cmdTypes);
		//初始化快捷键
		void initShortCut();
		//设置窗口位置
		void toDesk();
		void toTop();
		//设置菜单栏是否弹出
		bool getTopToolbarAutoPop();
		bool getBottomToolbarAutoPop();
		void setTopToolbarAutoPop(const bool& b);
		void setBottomToolbarAutoPop(const bool& b);

		bool getTopToolbarAutoPush();
		bool getBottomToolbarAutoPush();
		void setTopToolbarAutoPush(const bool& b);
		void setBottomToolbarAutoPush(const bool& b);

		//获取窗口被置于底层的状态
		bool getOnlyBottom();
		void setOnlyBottom(const bool& b);

		void setAcitonCommandCheck(const std::string& cmd, bool check);
		void addActionCommand();
		void addActionCommand(std::vector<ActionCommandTypes> cmdTypes, OriginUI::ToolBar* bar);
		void addActionCommand(const std::string& cmd, OriginUI::ToolBar* bar, const bool& visible);

		//获取、设置底部工具栏的状态
		void setBottomToolbarHide(const bool& b);
		bool getBottomToolbarHide();

		//获取真实的size() 如果在高dpi模式下被放大，依然会返回真实的大小
		QSize getRealSize();
		//载入设置
		virtual void loadSetting();
	private:
		void init();
	protected:
		void resizeEvent(QResizeEvent* event) override;
		void paintEvent(QPaintEvent* event) override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
		void enterEvent(QEnterEvent* event) override;
#else
		void enterEvent(QEvent* event) override;
#endif
		void leaveEvent(QEvent* event) override;
#if (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
		bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result)override;
#else
		bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;
#endif
		/* 初始化便笺所需要的一些命令
		*  这些命令，分别出现再上方的工具栏和下方的工具栏
		*  这些命令添加后并不会立即显示到工具栏，在调用addActionCommand()时，才会实际添加到工具栏
		*  并且在添加到工具栏之前，还会首先判断命令在设置中是否设置了是否显示
		*  如果状态为不显示，那么也不会显示到工具栏
		*  即使没有显示到工具栏，这里添加的命令也可以用过快捷键触发，如果有快捷键的情况下
		*/
		void initActionCommandToTop(const ActionCommandTypes& cmd);
		void initActionCommandToBottom(const ActionCommandTypes& cmd);
		/* 用于初始化派生类的命令
		*/
		virtual void initActionCommand() = 0;
		//获取上下工具栏
		OriginUI::ToolBar* getNavigaitonBar();
		OriginUI::ToolBar* getBottomBar();
		//设置便笺内容控件
		StickyNoteContent* getStickyNoteContent();
		void setStickyNoteContent(StickyNoteContent* content);
	private:
		StickyNoteD* d;

	public Q_SLOTS:
		void navigaitonBarMouseMove(QPoint pos);
		void contentGotFocus();
		void contentLostFocus();
		void contentChange();
		void colorChange(QColor color);
		void toolbarLeftButtonRelease();
		void shortcutActive();
	};
}
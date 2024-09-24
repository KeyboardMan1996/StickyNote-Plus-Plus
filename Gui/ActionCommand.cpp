#include "ActionCommand.h"
#include "NoteFile.h"
#include "pushpin.h"
#include "StickyNote.h"
#include "StickyNotePool.h"
#include "NoteTextEdit.h"
#include "HistoryWidget.h"
#include <QIcon>
#include <iostream>
#include <QCoreApplication>
#include <QEventLoop>
#include <QPixmap>
#include <QPainter>
#include <QColorDialog>
#include "LogTool.h"
#include "ToDoListContent.h"
#define CREAT_REATOR(className,cmdName)\
		class Creator##className:public ActionCreator{\
		public:\
			ActionCommand* CreatAction() override{\
					return new  className;\
			}\
			std::string getCmdName(){\
				return cmdName;\
			}\
		};

NGui::ActionCmdFactory::ActionCmdFactory()
{

}

std::shared_ptr<NGui::ActionCmdFactory> NGui::ActionCmdFactory::instance;

namespace NGui {

	ActionCmdFactory::~ActionCmdFactory()
	{
		for (auto iter = creators.begin(); iter != creators.end(); iter++)
		{
			delete iter->second;
		}
		creators.clear();
	}

	ActionCommand* ActionCmdFactory::creatActionCommand(const std::string& cmd)
	{
		auto iter = creators.find(cmd);
		if (iter == creators.end())
		{
			std::cerr << "not found cmd: " << cmd << std::endl;
			return nullptr;
		}
		auto action = iter->second->CreatAction();
		action->init();
		return action;
	}

	void ActionCmdFactory::registerCmd(ActionCreator* creator)
	{
		creators.insert(std::map<std::string, ActionCreator*>::value_type(creator->getCmdName(), creator));
	}

	void ActionCommand::setShortcut(const QString& shortcut)
	{
		this->shortcut = shortcut;
	}

	void ActionCommand::setToolTip(const QString& tip)
	{
		if (getShortcut().isEmpty())
		{
			OriginUI::Action::setToolTip(tip);
		}
		else {
			OriginUI::Action::setToolTip(tip + "(" + getShortcut() + +")");
		}
	}

	QString ActionCommand::getShortcut()
	{
		return shortcut;
	}

	ActionCommandTypes::ActionCommandTypes(const std::string& cmd, const bool& b)
		:cmd(cmd),visible(b)
	{

	}
#if 0 //暂时不使用宏
#define CREAT_STICKY_NOTE_TYPE_COMMAND(stickyNoteContent,nfStickyNote)\
	class ActionCommand##stickyNoteContent :public ActionCommand {\
	public:\
		bool isUsable(StickyNote* note) override\
		{\
			auto nfNote = note->getNFNote();\
			if (nfNote->stickyNoteType.getValue() == nfStickyNote::getStickyNoteType())\
				return true;\
			return false;\
		}\
		nfStickyNote* getNfNote(StickyNote* note) {\
			auto textNote = dynamic_cast<nfStickyNote*>(note);\
			if (PtrIsNull(textNote));\
			return textNote;\
		}\
		stickyNoteContent * getContent(StickyNote* note) {\
			auto textEdit = dynamic_cast<stickyNoteContent *>(note->getContent());\
			if (PtrIsNull(textEdit));\
			return textEdit;\
		}\
	};
	CREAT_STICKY_NOTE_TYPE_COMMAND(NoteTextEdit,NF::StickyNoteText);
	CREAT_STICKY_NOTE_TYPE_COMMAND(ToDoListContent, NF::StickyNoteToDoList);
#endif
	class ActionCommandContentText :public ActionCommand {
		
	public:
		bool isUsable(StickyNote* note) override
		{
			auto nfNote = note->getNFNote();
			if (nfNote->stickyNoteType.getValue() == NF::StickyNoteText::getStickyNoteType())
				return true;
			return false;
		}
		NF::StickyNoteText* getNfNote(StickyNote* note) {
			auto textNote = dynamic_cast<NF::StickyNoteText*>(note);
			if (PtrIsNull(textNote));
			return textNote;
		}
		NoteTextEdit* getContent(StickyNote* note) {
			auto textEdit = dynamic_cast<NoteTextEdit*>(note->getContent());
			if (PtrIsNull(textEdit));
			return textEdit;
		}
	};

	class ActionCommandToDoList :public ActionCommand {
	public:
		bool isUsable(StickyNote* note) override
		{
			auto nfNote = note->getNFNote();
			if (nfNote->stickyNoteType.getValue() == NF::StickyNoteToDoList::getStickyNoteType())
				return true;
			return false;
		}
		NF::StickyNoteToDoList* getNfNote(StickyNote* note) {
			auto todoNote = dynamic_cast<NF::StickyNoteToDoList*>(note);
			if (PtrIsNull(todoNote));
			return todoNote;
		}
		ToDoListContent* getContent(StickyNote* note) {
			auto textEdit = dynamic_cast<ToDoListContent*>(note->getContent());
			if (PtrIsNull(textEdit));
			return textEdit;
		}

	};


	class ActionPinIconLayer :public ActionCommand {
		Q_DECLARE_TR_FUNCTIONS(NGui::ActionPinIconLayer)
	public:
		void init()override {
			setCheckable(true);
			setCheckIcon(QIcon(":/icon/pinDesktopRed.svg"));
			setNoCheckIcon(QIcon(":/icon/pinDesktopBlack.svg"));
			setToolTip(tr("pin to icon layer"));
		}
		void active(StickyNote* note, const bool& check) override{
			if (!Pushpin::isDesktop(note))
			{
				Pushpin::toDesktop(note);
			}else {
				note->setOnlyBottom(false);
			}
		}
		void updateState(StickyNote* note) {
			if (Pushpin::isDesktop(note))
				setChecked(true);
			else
				setChecked(false);
		}
	};
	CREAT_REATOR(ActionPinIconLayer, "cmd_pin_icon_layer");

	class ActionClose :public ActionCommand {
		Q_DECLARE_TR_FUNCTIONS(NGui::ActionClose)
	public:
		void init() {
			setIcon(QIcon(":/icon/sticky_note_close.svg"));
			setToolTip(tr("close"));
		}
		void active(StickyNote* note, const bool& check) {
			auto pool = StickyNotePool::GetInstance();
			pool->StickyNoteCloseActionTirgger(note);
		}
	};
	CREAT_REATOR(ActionClose, "cmd_close");

	class ActionPinTop:public ActionCommand {
		Q_DECLARE_TR_FUNCTIONS(NGui::ActionPinTop)
	public:
		void init() {
			setCheckable(true);
			setCheckIcon(QIcon(":/icon/pinTopRed.svg"));
			setNoCheckIcon(QIcon(":/icon/pinTopBlack.svg"));
			setToolTip(tr("pin to top"));
		}
		void active(StickyNote* note, const bool& check) {
			if (Pushpin::isTop(note))
				Pushpin::toNormal(note);
			else if (Pushpin::isDesktop(note))
			{
				/*
				*	由于未显示过的窗口使用windowsAPi设置将窗口固定到最前
				*	会导致设置无效，所以需要延迟调用，在窗口显示之后再设置窗口的位置
				*/
				std::shared_ptr<NF::StickyNote> fnote = note->getNFNote();
				StickyNotePool::GetInstance()->closeNoteNoSave(note);
				fnote->state = NF::StickyNote::PIN_ALWAYS_TOP;

				auto newNote = StickyNotePool::CreatStickyNote(fnote);
				newNote->show();
				NoteTextEdit::getNoteTextEdit(newNote->getContent())->setFocus();
				newNote->saveNote();
				QCoreApplication::processEvents(QEventLoop::AllEvents,100);
				newNote->toTop();
				newNote->updateCmdState();
			}
			else
				note->toTop();
		}
		void updateState(StickyNote* note)
		{
			if (Pushpin::isTop(note))
				setChecked(true);
			else 
				setChecked(false);
		}
	};
	CREAT_REATOR(ActionPinTop, "cmd_pin_top");

	class ActionNewNote :public ActionCommand {
		Q_DECLARE_TR_FUNCTIONS(NGui::ActionNewNote)
	public:
		void init() {
			setIcon(QIcon(":/icon/add_note.svg"));
			setToolTip(tr("new sticky note"));
		}
		void active(StickyNote* note, const bool& check) {
			//调整窗口位置于当前窗口位置差不多
			auto widget = StickyNotePool::CreatStickyNote(
				note->getNFNote()->stickyNoteType.getValue());
			auto p = note->pos();
			//移动的位置
			int mv = 20;
			p.setX(p.x() - mv);
			p.setY(p.y() - mv);
			widget->move(p);
			widget->show();
		}
	};
	CREAT_REATOR(ActionNewNote, "cmd_new_note");

	class ActionColorSetting :public ActionCommand {
		Q_DECLARE_TR_FUNCTIONS(NGui::ActionColorSetting)
	public:
		void init() {
			setIcon(QIcon(":/icon/color.svg"));
			setToolTip(tr("set background color"));
		}
		void active(StickyNote* note, const bool& check){
			note->popColorSetting();
		}
	};
	CREAT_REATOR(ActionColorSetting, "cmd_color_setting");

	class ActionNewCopyStyle :public ActionCommand {
		Q_DECLARE_TR_FUNCTIONS(NGui::ActionNewCopyStyle)
	public:
		void init() {
			setIcon(QIcon(":/icon/copy.svg"));
			setToolTip(tr("copy to sticky note style,add new sticky note"));
		}
		void active(StickyNote* note, const bool& check) {
// 			auto textNote = 
// 			note->saveNote();
// 			auto nt = note->getNFNote()->copy();
// 
// 			//位置做一点偏移，否则窗口会重叠
// 			int mv = 20;
// 			nt.posX -= mv;
// 			nt.posY -= mv;
// 			nt.content = "";
// 			auto widget = StickyNotePool::CreatStickyNote(std::shared_ptr<NF::StickyNote>(new NF::StickyNote(nt)));
// 			widget->show();
		}
	};
	CREAT_REATOR(ActionNewCopyStyle, "cmd_new_copy_style");

	class ActionFontSizeUp :public ActionCommandContentText {
		Q_DECLARE_TR_FUNCTIONS(NGui::ActionFontSizeUp)
	public:
		void init() {
			setIcon(QIcon(":/icon/fontUp.svg"));
			setShortcut("PgUp");
			setToolTip(tr("up font size"));
		}

		void active(StickyNote* note, const bool& check) {
			getContent(note)->fontSizeUp();
		}
	};
	CREAT_REATOR(ActionFontSizeUp, "cmd_font_size_up");
	
	class ActionFontSizeLower :public ActionCommandContentText {
		Q_DECLARE_TR_FUNCTIONS(NGui::ActionFontSizeLower)
	public:
		void init() {
			setIcon(QIcon(":/icon/fontLower.svg"));
			setShortcut("PgDown");
			setToolTip(tr("down font size"));
		}
		void active(StickyNote* note, const bool& check) {
			getContent(note)->fontSizeDown();
		}
	};
	CREAT_REATOR(ActionFontSizeLower, "cmd_font_size_lower");

	class ActionTextMark :public ActionCommandContentText {
		Q_DECLARE_TR_FUNCTIONS(NGui::ActionTextMark)
	public:
		void init() {
			setCheckable(true);
			setNoCheckIcon(QIcon(":/icon/MarkerRed.svg"));
			setCheckIcon(QIcon(":/icon/Marker.svg"));
			setShortcut("Ctrl+M");
			setToolTip(tr("mark text"));
		}
		void active(StickyNote* note, const bool& check) {
			auto noteTextEdit = getContent(note);
			 if (!check)
 				noteTextEdit->clearBackground();
 			else {

 				QColor color(255, 0, 0, 100);
 				noteTextEdit->setBackground(color);
 			}
		}
		void updateState(StickyNote* note) {
			auto textedit = getContent(note);
			auto format = textedit->getTextCharFormat();
			if (format.background().color() == QColor(255, 0, 0, 100))
				setChecked(true);
			else
				setChecked(false);
		}
	};
	CREAT_REATOR(ActionTextMark, "cmd_text_mark");

	class ActionTextToCenter :public ActionCommandContentText {
		Q_DECLARE_TR_FUNCTIONS(NGui::ActionTextToCenter)
	public:
		void init() {
			setCheckable(true);
			setNoCheckIcon(QIcon(":/icon/font_center.svg"));
			setCheckIcon(QIcon(":/icon/font_center_red.svg"));
			setShortcut("Ctrl+E");
			setToolTip(tr("center"));
		}
		void active(StickyNote* note, const bool& check) {
			getContent(note)->setTextCenter(check);
		}
		void updateState(StickyNote* note) {
			setChecked(getContent(note)->getTextCenter());
		}
	};
	CREAT_REATOR(ActionTextToCenter, "cmd_text_to_center");

	class ActionTextToLeft :public ActionCommandContentText {
		Q_DECLARE_TR_FUNCTIONS(NGui::ActionTextToLeft)
	public:
		void init() {
			setCheckable(true);
			setNoCheckIcon(QIcon(":/icon/font_left.svg"));
			setCheckIcon(QIcon(":/icon/font_left_red.svg"));
			setShortcut("Ctrl+L");
			setToolTip(tr("left"));
		}
		void active(StickyNote* note, const bool& check) {
			getContent(note)->setTextLeft();
		}
		void updateState(StickyNote* note) {
			setChecked(getContent(note)->getTextLeft());
		}
	};
	CREAT_REATOR(ActionTextToLeft, "cmd_text_to_left");

	class ActionTextToRight :public ActionCommandContentText {
		Q_DECLARE_TR_FUNCTIONS(NGui::ActionTextToRight)
	public:
		void init() {
			setCheckable(true);
			setNoCheckIcon(QIcon(":/icon/font_right.svg"));
			setCheckIcon(QIcon(":/icon/font_right_red.svg"));
			setShortcut("Ctrl+R");
			setToolTip(tr("right"));
		}
		void active(StickyNote* note, const bool& check) {
			getContent(note)->setTextRight(check);
		}
		void updateState(StickyNote* note) {
			setChecked(getContent(note)->getTextRight());
		}
	};
	CREAT_REATOR(ActionTextToRight, "cmd_text_to_right");


	class ActionStrikeOutLine :public ActionCommandContentText {
		Q_DECLARE_TR_FUNCTIONS(NGui::ActionStrikeOutLine)
	public:
		void init() {
			setCheckable(true);
			setCheckIcon(QIcon(":/icon/strikeOutRed.svg"));
			setNoCheckIcon(QIcon(":/icon/strikeOutBlack.svg"));
			setShortcut("Ctrl+T");
			setToolTip(tr("delete line"));
		}
		void active(StickyNote* note, const bool& check) {
			getContent(note)->setStrikOut(check);
		}
		void updateState(StickyNote* note)
		{
			setChecked(getContent(note)->getStrikOut());
		}
	};
	CREAT_REATOR(ActionStrikeOutLine, "cmd_text_strike_out_line");
	
	class ActionShowHistory :public ActionCommand {
		Q_DECLARE_TR_FUNCTIONS(NGui::ActionShowHistory)
	public:
		void init() {
			setIcon(QIcon(":/icon/history.svg"));
			setToolTip(tr("display sticky note list"));
		}
		void active(StickyNote* note, const bool& check) {
			HistoryWidget::Getinstance()->show();
		}
	};
	CREAT_REATOR(ActionShowHistory, "cmd_show_history");

	class ActionFontItalic :public ActionCommandContentText {
		Q_DECLARE_TR_FUNCTIONS(NGui::ActionFontItalic)
	public:
		void init() {
			setCheckable(true);
			setNoCheckIcon(QIcon(":/icon/font_italic.svg"));
			setCheckIcon(QIcon(":/icon/font_italic_red.svg"));
			setShortcut("Ctrl+I");
			setToolTip(tr("font italic"));
		}
		void active(StickyNote* note, const bool& check) {
			getContent(note)->setItalic(check);
		}
		void updateState(StickyNote* note)
		{
			setChecked(getContent(note)->getItalic());
		}
	};
	CREAT_REATOR(ActionFontItalic, "cmd_font_italic");

	class ActionFontBold:public ActionCommandContentText {
		Q_DECLARE_TR_FUNCTIONS(NGui::ActionFontBold)
	public:
		void init() {
			setCheckable(true);
			setNoCheckIcon(QIcon(":/icon/font_blod.svg"));
			setCheckIcon(QIcon(":/icon/font_blod_red.svg"));
			setShortcut("Ctrl+B");
			setToolTip(tr("bold"));
		}
		void active(StickyNote* note, const bool& check) {
			getContent(note)->setFontBlod(check);
		}
		void updateState(StickyNote* note)
		{
			setChecked(getContent(note)->getFontBlod());
		}
	};
	CREAT_REATOR(ActionFontBold, "cmd_font_blod");

	class ActionFontUnderLine :public ActionCommandContentText {
		Q_DECLARE_TR_FUNCTIONS(NGui::ActionFontUnderLine)
	public:
		void init() {
			setCheckable(true);
			setNoCheckIcon(QIcon(":/icon/font_under_line.svg"));
			setCheckIcon(QIcon(":/icon/font_under_line_red.svg"));
			setShortcut("Ctrl+U");
			setToolTip(tr("font underline"));
		}
		void active(StickyNote* note, const bool& check) {
			getContent(note)->setUnderLine(check);
		}
		void updateState(StickyNote* note)
		{
			setChecked(getContent(note)->getUnderLine());
		}
	};
	CREAT_REATOR(ActionFontUnderLine, "cmd_font_under_line");

	class ActionFontList :public ActionCommandContentText {
		Q_DECLARE_TR_FUNCTIONS(NGui::ActionFontList)
	public:
		void init() {
			setCheckable(true);
			setNoCheckIcon(QIcon(":/icon/font_list_black.svg"));
			setCheckIcon(QIcon(":/icon/font_list_red.svg"));
			setToolTip(tr("list"));
		}
		void active(StickyNote* note, const bool& check) {
			getContent(note)->setListFormat();
		}
		void updateState(StickyNote* note)
		{
			setChecked(false);
		}
	};
	CREAT_REATOR(ActionFontList, "cmd_font_list");

	class ActionFontColor :public ActionCommandContentText {
		Q_DECLARE_TR_FUNCTIONS(NGui::ActionFontColor)
	public:
		void init() {
			setCheckable(false);
			iconPixmap.load(":/icon/font_color.png");
			color = Qt::black;
			setIcon(creatIcon(color));
			setToolTip(tr("font color"));
		}
		void active(StickyNote* note, const bool& check) {
			auto textEidt = getContent(note);
			QColorDialog colorDialog(color);
			if (!colorDialog.exec())
				return;
			QColor c = colorDialog.currentColor();
			if (c == color)
				return;

			textEidt->setFontColor(c);
			color = c;
			setIcon(creatIcon(color));
		}
		void updateState(StickyNote* note)
		{
			QColor c = getContent(note)->getFontColor();
			if (c == color)
				return;
			setIcon(creatIcon(c));
			color = c;
		}

		QIcon creatIcon(const QColor& color) {
			QPainter painter(&iconPixmap);
			painter.fillRect(0, iconPixmap.height() * 0.7, iconPixmap.width(), iconPixmap.height() * 0.4, color);
			return QIcon(iconPixmap);
		}
	private:
		QPixmap iconPixmap;
		QColor color;
	};

	CREAT_REATOR(ActionFontColor, "cmd_font_color");

	class ActionNewToDoList :public ActionCommandToDoList {
		Q_DECLARE_TR_FUNCTIONS(NGui::ActionNewToDoList)
	public:
		void init() {
			setCheckable(true);
			setNoCheckIcon(QIcon(":/icon/font_italic.svg"));
			setCheckIcon(QIcon(":/icon/font_italic_red.svg"));
			setToolTip(tr("new to do list"));
		}
		void active(StickyNote* note, const bool& check) {
			getContent(note)->addToDoListItem();
		}
		void updateState(StickyNote* note)
		{
			setChecked(false);
		}
	};
	CREAT_REATOR(ActionNewToDoList, "cmd_new_to_do_list");

	class ActionToDoListSplit :public ActionCommandToDoList {
		Q_DECLARE_TR_FUNCTIONS(NGui::AcitonToDoListSplit)
	public:
		void init() {
			setCheckable(true);
			setNoCheckIcon(QIcon(":/icon/font_italic.svg"));
			setCheckIcon(QIcon(":/icon/font_italic_red.svg"));
			setToolTip(tr("split item"));
		}
		void active(StickyNote* note, const bool& check) {
			auto b =  getContent(note)->getSplit();
			getContent(note)->setSplit(!b);
		}
		void updateState(StickyNote* note)
		{
			if (getContent(note)->getSplit())
				setChecked(true);
			else
				setChecked(false);
		}

	};
	CREAT_REATOR(ActionToDoListSplit, "cmd_to_do_list_split");


	class ActionToDoListEdit :public ActionCommandToDoList {
		Q_DECLARE_TR_FUNCTIONS(NGui::AcitonToDoListEdit)
	public:
		void init() {
			setCheckable(true);
			setNoCheckIcon(QIcon(":/icon/to_do_list_edit.svg"));
			setCheckIcon(QIcon(":/icon/to_do_list_edit_red.svg"));
			setToolTip(tr("edit item"));
		}
		void active(StickyNote* note, const bool& check) {
			auto b = getContent(note)->getEditState();
			getContent(note)->setEditState(!b);
		}
		void updateState(StickyNote* note)
		{
			if (getContent(note)->getEditState())
				setChecked(true);
			else
				setChecked(false);
		}

	};
	CREAT_REATOR(ActionToDoListEdit,"cmd_to_do_list_edit");
}

void NGui::ActionCmdFactory::registerAllCmd()
{
	registerCmd(new NGui::CreatorActionClose());
	registerCmd(new NGui::CreatorActionPinIconLayer());
	registerCmd(new NGui::CreatorActionPinTop());
	registerCmd(new NGui::CreatorActionColorSetting());
	registerCmd(new NGui::CreatorActionNewCopyStyle());
	registerCmd(new NGui::CreatorActionNewNote());
	registerCmd(new NGui::CreatorActionFontSizeLower());
	registerCmd(new NGui::CreatorActionFontSizeUp());
	registerCmd(new NGui::CreatorActionTextMark());
	registerCmd(new NGui::CreatorActionStrikeOutLine());
	registerCmd(new NGui::CreatorActionTextToCenter());
	registerCmd(new NGui::CreatorActionShowHistory());
	registerCmd(new NGui::CreatorActionFontItalic());
	registerCmd(new NGui::CreatorActionFontBold());
	registerCmd(new NGui::CreatorActionFontUnderLine());
	registerCmd(new NGui::CreatorActionTextToLeft());
	registerCmd(new NGui::CreatorActionTextToRight());
	registerCmd(new NGui::CreatorActionFontList());
	registerCmd(new NGui::CreatorActionFontColor());
	registerCmd(new NGui::CreatorActionNewToDoList());
	registerCmd(new NGui::CreatorActionToDoListSplit());
	registerCmd(new NGui::CreatorActionToDoListEdit());
}

std::vector<std::string> NGui::ActionCmdFactory::getAllCmdName()
{
	std::vector<std::string> cmds;
	for (auto iter : creators)
	{
		cmds.push_back(iter.first);
	}

	return cmds;
}

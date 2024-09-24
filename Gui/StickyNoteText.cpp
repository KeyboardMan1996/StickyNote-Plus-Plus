#include "StickyNoteText.h"
#include "NoteTextEdit.h"
#include "LogTool.h"
#include "Setting.h"
NGui::StickyNoteText::StickyNoteText(std::shared_ptr<NF::StickyNote> note, QWidget* parent /*= 0*/)
	:StickyNote(note, new NoteTextEdit(), parent)
{
	initActionCommand();
}

NGui::StickyNoteText::~StickyNoteText()
{

}

void NGui::StickyNoteText::setText(const QString& text)
{
	auto textContent = dynamic_cast<NoteTextEdit*>(getContent());
	PtrIsNull(textContent);
	textContent->setText(text);
	saveNote();
}

void NGui::StickyNoteText::setHtml(const QString& html)
{
	auto textContent = dynamic_cast<NoteTextEdit*>(getContent());
	PtrIsNull(textContent);

	auto  cursor = textContent->textCursor();
	textContent->setHtml(html);
	textContent->setTextCursor(cursor);
	saveNote();
}

void NGui::StickyNoteText::loadSetting()
{
	StickyNote::loadSetting();
	auto setting = Setting::GetInstance();
	auto textEdit = dynamic_cast<NoteTextEdit*>(getContent());
	PtrIsNull(textEdit);
	if (textEdit)
	{
		QFont font;
		font.fromString(QString::fromStdString(setting->stickyNoteFont->getValue()));
		textEdit->setFont(font);
		textEdit->setAutoOpenUrl(setting->visitiUrl->getValue());
	}

}

void NGui::StickyNoteText::initActionCommand()
{
	//初始化按钮
	initActionCommandToTop(ActionCommandTypes("cmd_new_note", true));
#if 0 //暂时取消一些掉命令
	initActionCommandToTop(ActionCommandTypes("cmd_new_copy_style", true));
#endif
	initActionCommandToTop(ActionCommandTypes("cmd_color_setting", true));
	initActionCommandToTop(ActionCommandTypes("cmd_show_history", true));
	initActionCommandToTop(ActionCommandTypes("cmd_pin_icon_layer", true));
	initActionCommandToTop(ActionCommandTypes("cmd_pin_top", true));
	initActionCommandToTop(ActionCommandTypes("cmd_close", true));

	initActionCommandToBottom(ActionCommandTypes("cmd_font_size_up", false));
	initActionCommandToBottom(ActionCommandTypes("cmd_font_size_lower",false));
	initActionCommandToBottom(ActionCommandTypes("cmd_text_mark", true));
	initActionCommandToBottom(ActionCommandTypes("cmd_font_blod", true));
	initActionCommandToBottom(ActionCommandTypes("cmd_font_italic", false));
	initActionCommandToBottom(ActionCommandTypes("cmd_font_under_line", false));
	initActionCommandToBottom(ActionCommandTypes("cmd_text_strike_out_line", true));
	initActionCommandToBottom(ActionCommandTypes("cmd_text_to_center", true));
	initActionCommandToBottom(ActionCommandTypes("cmd_text_to_left", false));
	initActionCommandToBottom(ActionCommandTypes("cmd_text_to_right", false));
	initActionCommandToBottom(ActionCommandTypes("cmd_font_list", false));
	initActionCommandToBottom(ActionCommandTypes("cmd_font_color", false));
}


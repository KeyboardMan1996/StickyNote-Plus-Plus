#include "StickyNoteItemContent.h"
#include <QScrollBar>
#include "StickyNoteItem.h"
#include "LogTool.h"
#include <QTextDocument>
#include <QTextBlock>
#include <QAbstractTextDocumentLayout>
NGui::StickyNoteItemTextContent::StickyNoteItemTextContent(QWidget* parent /*= 0*/)
	:QTextEdit(parent)
{
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	filter = new StickyNoteItemEventFilter;
	installEventFilter(filter);
	verticalScrollBar()->installEventFilter(filter);
	setReadOnly(true);
}

NGui::StickyNoteItemTextContent::~StickyNoteItemTextContent()
{
	delete filter;
}

void NGui::StickyNoteItemTextContent::loadContent(const std::shared_ptr<NF::StickyNote> nfNote)
{
	if (PtrIsNull(nfNote))
		return;
	auto textNode = std::dynamic_pointer_cast<NF::StickyNoteText>(nfNote);
	if (PtrIsNull(textNode))
		return;
	setHtml(QString::fromStdString(textNode->contentText.getValue()));
}

QSize NGui::StickyNoteItemTextContent::getMiniSize()
{
	auto doc = document();

	const int maxHieght = 200;
	const int blockCount = 6;	//最多显示多少行
	int count = 0;
	const int barHieght = 70;	//布局的额外宽度
	int h = barHieght;

	for (auto block = doc->begin();
		block != doc->end() && count < blockCount;
		block = block.next())
	{
		h += doc->documentLayout()->blockBoundingRect(block).height();
	}

	h = h > maxHieght ? maxHieght : h;
	return QSize(0, h);
}

QString NGui::StickyNoteItemTextContent::getContentText()
{
	return toPlainText();
}

void NGui::StickyNoteItemTextContent::resizeEvent(QResizeEvent* e)
{
	QTextEdit::resizeEvent(e);
	verticalScrollBar()->setValue(0);
}

NGui::StickyNoteItemContent* NGui::StickyNoteItemContentCraeter::creatStickyNoteItem(const std::shared_ptr<NF::StickyNote> nfNote)
{
	if (nfNote->stickyNoteType.getValue() == "stickyNoteText")
	{
		return new StickyNoteItemTextContent();
	}
	else if (nfNote->stickyNoteType.getValue() == "contentToDoList")
	{
		return new StickyNoteItemItemToDoList();
	}
	else {
		return new StickyNoteItemTextContent();
	}
}

NGui::StickyNoteItemItemToDoList::StickyNoteItemItemToDoList(QWidget* parent /*= 0*/)
	:ToDoListContent(parent)
{
	filter = new StickyNoteItemEventFilter;
	installEventFilter(filter);
	verticalScrollBar()->installEventFilter(filter);

	// 禁用子控件的交互
	setEnabled(false);


}

NGui::StickyNoteItemItemToDoList::~StickyNoteItemItemToDoList()
{
	delete filter;
}

void NGui::StickyNoteItemItemToDoList::loadContent(const std::shared_ptr<NF::StickyNote> nfNote)
{
	ToDoListContent::loadNote(nfNote);
}

QSize NGui::StickyNoteItemItemToDoList::getMiniSize()
{
	int height = 0;
	if (getToDoListCount() > 3)
	{
		height = 3 * getToDoListSize().height() + 70;
	}else {
		height = getToDoListCount() * getToDoListSize().height() + 70;
	}

	return QSize(0, height);
}

QString NGui::StickyNoteItemItemToDoList::getContentText()
{
	auto listText = getToDOListText();
	QString str = "";
	for (auto iter = listText.begin(); iter != listText.end(); iter++)
	{
		str += *iter;
	}
	return str;
}

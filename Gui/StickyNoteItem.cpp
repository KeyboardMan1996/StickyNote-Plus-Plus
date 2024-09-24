#include "StickyNoteItem.h"
#include "NoteTextEdit.h"
#include "StickyNotePool.h"
#include "StickyNote.h"
#include "NoteFile.h"
#include "EventManager.h"
#include "StickyNoteContentChangeEvent.h"
#include <QToolButton>
#include <qpainter.h>
#include <QTextDocument>
#include <QTextBlock>
#include <QAbstractTextDocumentLayout>
#include <QScrollBar>
#include <QDebug>
#include <QApplication>
#include "LogTool.h"
#include "StickyNoteItemContent.h"

NGui::StickyNoteItem::StickyNoteItem(StickyNoteItemContent* ItemContent, QWidget* parent /*= 0*/)
	:QWidget(parent)
{

	//初始化数据
	filter = new StickyNoteItemEventFilter();
	backColor = QColor("#FFF7D1");
	isHover = false;
	hoverColor = QColor(0, 0, 0, 40);
	setMinimumSize(40, 50);

	//初始化布局
	vLayout = new QVBoxLayout();
	hLayout = new QHBoxLayout();
	
	content = ItemContent;
	vLayout->addLayout(hLayout);
	auto contentWidget = dynamic_cast<QWidget*>(content);
	PtrIsNull(contentWidget);
	vLayout->addWidget(contentWidget);
	setLayout(vLayout);

	actionShowMenu = new QAction();
	actionShowMenu->setToolTip(tr("Menu"));
	auto button = new QToolButton(this);
	button->setDefaultAction(actionShowMenu);
	hLayout->addStretch();
	hLayout->addWidget(button);
	button->hide();

	timesLabel = new QLabel();
	hLayout->addWidget(timesLabel);

	actionDeleteNote = new QAction(tr("delete"));
	actionDeleteNote->setToolTip(tr("delete sticky note"));
	connect(actionDeleteNote, SIGNAL(triggered(bool)), this, SLOT(actionTrigger(bool)));

	actionRestNote = new QAction(tr("rest position"));
	actionRestNote->setToolTip(tr("rest sticky note position"));
	connect(actionRestNote, SIGNAL(triggered(bool)), this, SLOT(actionTrigger(bool)));

	actionShowNote = new QAction(tr("display"));
	actionShowNote->setToolTip(tr("display sticky note in windows"));
	connect(actionShowNote, SIGNAL(triggered(bool)), this, SLOT(actionTrigger(bool)));

	actionCloseNote = new QAction(tr("close"));
	actionCloseNote->setToolTip(tr("close sticky note"));
	connect(actionCloseNote, SIGNAL(triggered(bool)), this, SLOT(actionTrigger(bool)));

	menu = new QMenu();
	menu->addAction(actionShowNote);
	menu->addAction(actionCloseNote);
	menu->addAction(actionDeleteNote);
	menu->addAction(actionRestNote);
}

NGui::StickyNoteItem::~StickyNoteItem()
{
	vLayout->deleteLater();
	hLayout->deleteLater();
	actionShowMenu->deleteLater();
	actionDeleteNote->deleteLater(); 
	actionCloseNote->deleteLater(); 
	actionShowNote->deleteLater();
	actionRestNote->deleteLater();
	menu->deleteLater();
	timesLabel->deleteLater();
	delete content;
}

void NGui::StickyNoteItem::setFileNote(const std::shared_ptr<NF::StickyNote> note)
{
	if (PtrIsNull(note))
		return;
	auto c = QColor(QString::fromStdString(note->backgroundColor));
	if (c.isValid())
		backColor = c;
	loadNoteContent(note);
	timesLabel->setText(QString::fromStdString(note->times));
	this->fileNote = note;
	autoSize();
}

std::shared_ptr<NF::StickyNote> NGui::StickyNoteItem::getFileNote()
{
	return fileNote;
}


void NGui::StickyNoteItem::loadNoteContent(const std::shared_ptr<NF::StickyNote> note)
{
	content->loadContent(note);
}

void NGui::StickyNoteItem::setBackgroundColor(const QColor& c)
{
	backColor = c;
	backColor.setAlpha(255);
	update();
}

QString NGui::StickyNoteItem::getText()
{
	return content->getContentText();
}

void NGui::StickyNoteItem::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setBrush(QBrush(backColor));
	painter.setPen(backColor);
	painter.drawPolygon(polygon);

	const QColor triangleColor(0, 0, 0, 40);
	painter.setBrush(QBrush(triangleColor));
	painter.drawPolygon(triangle);

	//如果鼠标悬停增加一层透明色
	if (isHover)
	{
		const QColor pen(0, 0, 0, 0);
		painter.setPen(pen);
		painter.setBrush(QBrush(hoverColor));
		painter.drawPolygon(polygon);
	}
		

	QWidget::paintEvent(event);
}

void NGui::StickyNoteItem::mousePressEvent(QMouseEvent* event)
{
	auto p = event->globalPos();
	menu->move(p);
	menu->show();
}
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void NGui::StickyNoteItem::enterEvent(QEnterEvent* event)
#else
void NGui::StickyNoteItem::enterEvent(QEvent* event)
#endif
{
	QWidget::enterEvent(event);
	isHover = true;
	update();
}

void NGui::StickyNoteItem::leaveEvent(QEvent* event)
{
	QWidget::leaveEvent(event);
	isHover = false;
	update();
}

void NGui::StickyNoteItem::focusOutEvent(QFocusEvent* event)
{
	menu->close();
}

void NGui::StickyNoteItem::resizeEvent(QResizeEvent* event)
{
	QWidget::resizeEvent(event);

	auto re = this->rect();

	//三角位置大小
	const int trs = 15;

	polygon.clear();
	polygon<< QPoint(0,0) << QPoint(re.width(),0) << QPoint(re.width(),re.height() - trs)
		<< QPoint(re.width() - trs,re.height()) << QPoint(0,re.height()) << QPoint(0,0);

	triangle.clear();
	triangle << QPoint(re.width() - trs, re.height() - trs) << QPoint(re.width(), re.height() - trs)
		<< QPoint(re.width() - trs, re.height()) << QPoint(re.width() - trs, re.height() - trs);
}

void NGui::StickyNoteItem::showEvent(QShowEvent* event)
{
	QWidget::showEvent(event);
	/*
		由于documentLayout()->blockBoundingRect()函数，需要再窗口显示出来之后才能计算行高度。
		在窗口未显示之前计算的高度是错误的。
		所以在窗口显示时需要从新计算一下高度
	*/
	autoSize();
}

void NGui::StickyNoteItem::showNoteWidget()
{
	auto pool = StickyNotePool::GetInstance();
	//如果已经显示，则将窗口显示于顶层
	if (pool->hasFileNote(fileNote->guid))
	{
		auto nWidget = pool->getStickyNote(fileNote->guid);
		nWidget->raise();
		return;
	}
	fileNote->visible = true;
    auto noteWidget = pool->CreatStickyNote(fileNote);
	noteWidget->show();
	noteWidget->saveNote();
}

void NGui::StickyNoteItem::closeNoteWidget()
{
	auto pool = StickyNotePool::GetInstance();
	pool->hideStickyNote(fileNote->guid);
}

void NGui::StickyNoteItem::deleteNote()
{
	EventManager::postEvent(new StickyNoteDeleteEvent(getFileNote()));
}

void NGui::StickyNoteItem::autoSize()
{
	Q_EMIT listItemResize(content->getMiniSize());
}

void NGui::StickyNoteItem::actionTrigger(bool)
{
	if (sender() == actionShowMenu)
	{

	}else if (sender() == actionShowNote)
	{
		showNoteWidget();
	}else if (sender() == actionCloseNote)
	{
		closeNoteWidget();
	}else if (sender() == actionDeleteNote) {
		deleteNote();
	}
}

bool NGui::StickyNoteItemEventFilter::eventFilter(QObject* watched, QEvent* event)
{
	if (event->type() == QEvent::Wheel)
	{
		if (watched->parent()) {
			QApplication::sendEvent(watched->parent(), event);
		}
		return true;
	}
	return QObject::eventFilter(watched, event);
}

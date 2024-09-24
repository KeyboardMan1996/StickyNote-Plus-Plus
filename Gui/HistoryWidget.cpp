#include "HistoryWidget.h"
#include "NavigationBar.h"
#include "InputLineEdit.h"
#include "StickyNoteItem.h"
#include "StickyNoteContentChangeEvent.h"
#include "EventManager.h"
#include "StickyNotePool.h"
#include "StickyNote.h"
#include "Setting.h"
#include "qxtglobalshortcut.h"
#include "SettingUI.h"
#include <mutex>
#include <QListWidgetItem>
#include <QApplication>
#include <QPainter>
#include <QScrollBar>
#include <QLineEdit>
#include <QRegularExpressionMatch>
#include <QRegularExpression>
#include <iostream>
#include <QMessageBox>
#include "LogTool.h"
#include "StickyNoteItemContent.h"
std::shared_ptr<NGui::HistoryWidget> NGui::HistoryWidget::instance;


void NGui::HistoryWidget::actionTrigger(bool b)
{
	if (sender() == actionClose)
	{
		this->close();
	}else if (sender() == actionNew) {
		auto note = StickyNotePool::CreatStickyNote("contentText");
		note->show();
	}else if (sender() == actionSetting)
	{
		SettingUI* setting = new SettingUI();
		setting->setAttribute(Qt::WA_DeleteOnClose);
		setting->show();
	}
}

void NGui::HistoryWidget::serchEditButtonClicked(bool b)
{
	//实现搜索功能
	if (sender() == serchEdit->getCancelButton())
	{
		serchEdit->getLineEdit()->clear();
		for (auto iter = listItemMap.begin(); iter != listItemMap.end(); iter++)
		{
			iter->second->setHidden(false);
		}
	}else {
		auto word = serchEdit->getLineEdit()->text();
		if (word.isEmpty())
			return;
		QRegularExpression exp(word + ".+");
		
		for (auto iter = listItemMap.begin(); iter != listItemMap.end(); iter++)
		{
			auto text = iter->first->getText();
			auto mach = exp.match(text);
			if (!mach.hasMatch())
				iter->second->setHidden(true);
		}
	}
}

void NGui::HistoryWidget::listItemResize(QSize size)
{
	auto stickyItem = dynamic_cast<StickyNoteItem*>(sender());
	if (!stickyItem)
		return;
	auto iter = listItemMap.find(stickyItem);
	if (iter == listItemMap.end())
		return;
	iter->second->setSizeHint(size);
}

void NGui::HistoryWidget::customEvent(QEvent* event)
{	switch (event->type())
	{

	case CONTENT_CHANGE:
	{
		auto e = dynamic_cast<StickyNoteContentChangeEvent*>(event);
		if (PtrIsNull(e))
			return;
		auto item = noteItems.find(e->note->guid);
		if (item == noteItems.end())
			return;
		item->second->loadNoteContent(e->note);
	}
		break;
	case COLOR_CHANGE:
	{
		auto e = dynamic_cast<StickyNoteColorChangeEvent*>(event);
		if (PtrIsNull(e))
			return;
		auto item = noteItems.find(e->note->guid);
		if (item == noteItems.end())
			return;
		item->second->setBackgroundColor(e->color);
	}
		break;
	case STICKY_NOTE_COLSE:
	{
		auto e = dynamic_cast<StickyNoteColseEvent*>(event);
		if (PtrIsNull(e))
			return;
		if (!e->note->isSaved())
			removeFileNote(e->note);
	}
		break;
	case STICKY_NOTE_CREAT:
	{
		auto e = dynamic_cast<StickyNoteCreatEvent*>(event);
		if (PtrIsNull(e))
			return;
		addFileNote(e->note);
	}
		break;
	case STICKY_NOTE_DELETE:
	{
		auto e = dynamic_cast<StickyNoteDeleteEvent*>(event);
		if (PtrIsNull(e))
			return;
		auto pool = StickyNotePool::GetInstance();
		pool->closeStickyNote(e->note->guid);
		removeFileNote(e->note);
		e->note->deleteFile();
	}
		break;
	}

}

void NGui::HistoryWidget::paintEvent(QPaintEvent* event)
{
#ifdef _RUN_WITH_WIN7_
	OriginUI::WidgetNoBoder::paintEvent(event);
	QPainter painter(this);
	const QColor color(200,200,200,100);
	painter.setBrush(QColor(0, 0, 0, 0));
	painter.setPen(color);
	auto rect = this->rect();
	rect.setWidth(rect.width() - 1);
	rect.setHeight(rect.height() - 1);
	painter.drawRect(rect);
#else
	NativeWindowTemplate<QWidget>::paintEvent(event);
#endif




}

void NGui::HistoryWidget::initGui()
{
	setMinimumSize(400, 500);

	vLayout = new QVBoxLayout();
	vLayout->setSpacing(10);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	vLayout->setContentsMargins(0, 0, 0, 0);
#else
	vLayout->setMargin(0);
#endif
	
	setLayout(vLayout);
	navigationBar = new OriginUI::NavigationBar();
	navigationBar->setMinimumHeight(40);
	navigationBar->setToolButtonMiniSize(QSize(40, 40));
	navigationBar->setIconSize(QSize(20, 20));
#ifndef _RUN_WITH_WIN7_
	NativeWindowTemplate<QWidget>::setTitleBar(navigationBar);
#else
	connect(navigationBar, SIGNAL(mouseMove(QPoint)), this, SLOT(navigationBarMouseMove(QPoint)));
#endif
	actionSetting = new QAction();
	actionSetting->setIcon(QIcon(":icon/setting.svg"));
	actionSetting->setToolTip(tr("setting"));
	connect(actionSetting, SIGNAL(triggered(bool)), this, SLOT(actionTrigger(bool)));
	navigationBar->addRightWidget(actionSetting);

	actionClose = new QAction();
	actionClose->setIcon(QIcon(":icon/off.svg"));
	actionClose->setToolTip(tr("close"));
	connect(actionClose, SIGNAL(triggered(bool)), this, SLOT(actionTrigger(bool)));
	navigationBar->addRightWidget(actionClose);

	actionNew = new QAction();
	actionNew->setIcon(QIcon(":/icon/add_note.svg"));
	actionNew->setToolTip(tr("new sticky note"));
	connect(actionNew, SIGNAL(triggered(bool)), this, SLOT(actionTrigger(bool)));
	navigationBar->addLeftWidget(actionNew);

	serchEdit = new OriginUI::InputLineEdit();
	serchEdit->setObjectName("serchEdit");
	serchEdit->setButtonLaoutMod(OriginUI::BUTTON_RIGHT);
	serchEdit->getLineEdit()->setPlaceholderText(tr("Search..."));
	serchEdit->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	serchEdit->setContentsMargins(2, 0, 0, 2);
	auto oktb = serchEdit->getOkButton();
	oktb->setIcon(QIcon(":/icon/search.svg"));
	oktb->setIconSize(QSize(20, 20));
	oktb->setToolTip(tr("Search"));
	auto ctb = serchEdit->getCancelButton();
	ctb->setIcon(QIcon(":/icon/off.svg"));
	ctb->setIconSize(QSize(20, 20));
	ctb->setToolTip(tr("clear search"));
	connect(oktb, SIGNAL(clicked(bool)), this, SLOT(serchEditButtonClicked(bool)));
	connect(ctb, SIGNAL(clicked(bool)), this, SLOT(serchEditButtonClicked(bool)));


	listWidget = new HistoryList();
	listWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	listWidget->setSortingEnabled(true);

	layout()->addWidget(navigationBar);
	layout()->addWidget(serchEdit);
	layout()->addWidget(listWidget);

}

void NGui::HistoryWidget::intitShortcut()
{
	auto setting = Setting::GetInstance();
	QKeySequence key(QString::fromStdString(setting->historyWidgetShortcut->getValue()));
	shortcut = new QxtGlobalShortcut(this);

	//当快捷键被出发时，显示历史窗口，并置顶
	connect(shortcut, &QxtGlobalShortcut::activated,
		[=]() {
			this->show();
			this->raise();
		});
	setShortcut(key);
}

void NGui::HistoryWidget::addFileNote(const std::shared_ptr<NF::StickyNote> note)
{
	if (hasNote(note))
		return;
	auto noteItem = new  StickyNoteItem(StickyNoteItemContentCraeter::creatStickyNoteItem(note));
	connect(noteItem, SIGNAL(listItemResize(QSize)), this, SLOT(listItemResize(QSize)));

	auto item = new ListWidgetItem();
	item->setText(QString::fromStdString(note->times));
	listWidget->addItem(item);
	listWidget->setItemWidget(item, noteItem);

	noteItems.insert(StickyNoteItemMap::value_type(note->guid, noteItem));
	listItemMap.insert(QListWidgetItemMap::value_type(noteItem, item));

	//等待关系绑定完成，载入时可以通过信号初始化item大小
	noteItem->setFileNote(note);
	//排序
	listWidget->sortItems(Qt::DescendingOrder);
}

void NGui::HistoryWidget::removeFileNote(std::shared_ptr<NF::StickyNote> note)
{
	//从items中移除
	auto iter = noteItems.find(note->guid);
	if (iter == noteItems.end())
		return;
	auto item = iter->second;
	item->deleteLater();
	noteItems.erase(iter);

	auto itemIter = listItemMap.find(item);
	if (itemIter != listItemMap.end())
	{
		auto row = listWidget->row(itemIter->second);
		listWidget->takeItem(row);
		delete itemIter->second;
		listItemMap.erase(itemIter);
	}
}

bool NGui::HistoryWidget::hasNote(std::shared_ptr<NF::StickyNote> note)
{
	auto iter = noteItems.find(note->guid);
	if (iter == noteItems.end())
		return false;
	return true;
}

void NGui::HistoryWidget::postEvent(QEvent* event)
{
	if (event->type() != CONTENT_CHANGE && 
		event->type() != COLOR_CHANGE&&
		event->type() != STICKY_NOTE_COLSE&&
		event->type() != STICKY_NOTE_CREAT&&
		event->type() != STICKY_NOTE_DELETE)
		return;
	QApplication::postEvent(this, event);
}

/**
* 设置呼出的快捷键
* @brief NGui::HistoryWidget::setShortcut
* @param const QKeySequence & key
* @return void
*/
void NGui::HistoryWidget::setShortcut(const QKeySequence& key)
{
	try
	{
		shortcut->setShortcut(key);
	}catch (std::string str)
	{
		std::cerr << str << std::endl;
		int ret = QMessageBox::warning(this, tr("StickyNote"),
			tr("Shortcut key registration conflict when invoking the sticky note list window. \n"
				"Please set a new shortcut key."),
			QMessageBox::Ok);
	}
}

NGui::HistoryWidget::~HistoryWidget()
{
	noteItems.clear();
	listItemMap.clear();

	
	delete navigationBar;
	delete serchEdit;
	delete listWidget;
	delete vLayout;
	delete actionClose;
}

NGui::HistoryWidget::HistoryWidget()
{
	initGui();
	intitShortcut();
}

std::shared_ptr<NGui::HistoryWidget> NGui::HistoryWidget::Getinstance()
{
	static std::once_flag flag;
	std::call_once(flag, [&] {
		instance.reset(new HistoryWidget());
		//注册事件发送
		EventManager::GetInstance()->rgisterSender(instance.get());
		});

	return instance;
}

void NGui::HistoryWidget::destoryInstance()
{
	instance.reset();
}

#ifdef _RUN_WITH_WIN7_
void NGui::HistoryWidget::navigationBarMouseMove(QPoint p)
{
	p += this->pos();
	this->move(p);
}
#endif

bool NGui::ListWidgetItem::operator>(const QListWidgetItem& item)
{
	if (text() > item.text())
		return true;
	return false;
}

bool NGui::ListWidgetItem::operator==(const QListWidgetItem& item)
{
	if (text() == item.text())
		return true;
	return false;
}

bool NGui::ListWidgetItem::operator<(const QListWidgetItem& item)
{
	if (text() < item.text())
		return true;
	return false;
}

NGui::HistoryList::HistoryList(QWidget* parent /*= 0*/)
	:QListWidget(parent)
{

}

void NGui::HistoryList::wheelEvent(QWheelEvent* e)
{
	QScrollBar* bar = this->verticalScrollBar();

	int len = e->angleDelta().y();
	bar->setValue(bar->value() - 0.2 * len);


}

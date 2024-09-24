#include "ToDoListContent.h"
#include <QListWidgetItem>
#include "ToDoListItem.h"
#include <map>
#include "StickyNote.h"
#include "LogTool.h"
#include "widgetSplitter.h"
namespace NGui {
	class ToDoListContentD {
	public:
		ToDoListContentD();
		~ToDoListContentD();

		std::map<QListWidgetItem*, ToDoListItem*> ItemWidgetMap;
		QListWidgetItem* addItem,*finishedSplitterItem,*notFinishedSplitterItem;
		ToDoListAddItemWidget* addItemWidget;
		OriginUI::WidgetSplitter* finishedSplitterWidget, * notFinishedSplitterWidget;
		bool isSplit,eidtState;
	};

	ToDoListContentD::ToDoListContentD()
	{
		addItem = new QListWidgetItem();
		addItemWidget = new ToDoListAddItemWidget();
		//addItemWidget->setMinimumHeight(40);
		addItem->setSizeHint(addItemWidget->sizeHint());


		finishedSplitterItem = new QListWidgetItem();
		notFinishedSplitterItem = new QListWidgetItem();

		finishedSplitterWidget = new OriginUI::WidgetSplitter();
		finishedSplitterWidget->setText("Finished");
		notFinishedSplitterWidget = new OriginUI::WidgetSplitter();
		notFinishedSplitterWidget->setText("Not finished");

		finishedSplitterItem->setSizeHint(finishedSplitterWidget->sizeHint());
		notFinishedSplitterItem->setSizeHint(notFinishedSplitterWidget->sizeHint());

		isSplit = false;
		eidtState = false;
	}

	ToDoListContentD::~ToDoListContentD()
	{
		delete addItem;
		delete addItemWidget;
		delete notFinishedSplitterItem;
		delete finishedSplitterItem;
		delete notFinishedSplitterWidget;
		delete finishedSplitterWidget;
		for (auto iter = ItemWidgetMap.begin(); iter != ItemWidgetMap.end(); iter++)
		{
			delete iter->first;
			delete iter->second;
		}
	}

}

NGui::ToDoListContent::ToDoListContent(QWidget* parent /*= 0*/)
	:QListWidget(parent),d(new ToDoListContentD())
{
	//添加新建代办事件项按钮
	addItem(d->addItem);
	setItemWidget(d->addItem, d->addItemWidget);
	connect(d->addItemWidget, SIGNAL(clicked()), this, SLOT(addItemButtonClicked()));
	// 禁用垂直滚动条
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	// 禁用水平滚动条
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	setFocusPolicy(Qt::NoFocus);

}

NGui::ToDoListContent::~ToDoListContent()
{
	delete d;
}

void NGui::ToDoListContent::setEditState(const bool& state)
{
	d->eidtState = state;
	if (state)
	{
		for (auto iter : d->ItemWidgetMap)
		{
			iter.second->showEditButton();
		}
	}
	else {
		for (auto iter : d->ItemWidgetMap)
		{
			iter.second->hideEditButton();;
		}
	}
}

bool NGui::ToDoListContent::getEditState()
{
	return d->eidtState;
}

void NGui::ToDoListContent::loadNote(const std::shared_ptr<NF::StickyNote> note)
{
	auto todoListNote = std::dynamic_pointer_cast<NF::StickyNoteToDoList>(note);
	if (PtrIsNull(todoListNote))
		return;
	for (auto data : todoListNote->toDoListItems)
	{
		addToDoListItem(data);
	}
}

void NGui::ToDoListContent::saveNote(const std::shared_ptr<NF::StickyNote> note)
{
	auto todoListNote = std::dynamic_pointer_cast<NF::StickyNoteToDoList>(note);
	if (PtrIsNull(todoListNote))
		return;
	todoListNote->toDoListItems.clear();
	for (int i = 0; i < count(); i++)
	{
		auto listItem = item(i);
		auto widget = itemWidget(listItem);
		auto toDoListWidget = dynamic_cast<ToDoListItem*>(widget);
		if(!toDoListWidget)
			continue;
		todoListNote->toDoListItems.push_back(toDoListWidget->getData());
	}

}

void NGui::ToDoListContent::connectStickyNote(StickyNote* stickyNote)
{
	connect(this, SIGNAL(gotFocus()), stickyNote, SLOT(contentGotFocus()));
	connect(this, SIGNAL(lostFocus()), stickyNote, SLOT(contentLostFocus()));
}

void NGui::ToDoListContent::disConnectStickyNote(StickyNote* stickyNote)
{
	disconnect(this, SIGNAL(gotFocus()), stickyNote, SLOT(contentGotFocus()));
	disconnect(this, SIGNAL(lostFocus()), stickyNote, SLOT(contentLostFocus()));
}

bool NGui::ToDoListContent::isEmpty()
{
	if (d->ItemWidgetMap.size() != 0)
		return false;
	return true;
}

void NGui::ToDoListContent::initGui(StickyNote* stickyNote)
{
	stickyNote->setBottomToolbarAutoPop(true);
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void NGui::ToDoListContent::enterEvent(QEnterEvent* event)
#else
void NGui::ToDoListContent::enterEvent(QEvent* event)
#endif
{
	QListWidget::enterEvent(event);
	//Q_EMIT gotFocus();
}

void NGui::ToDoListContent::leaveEvent(QEvent* event)
{
	QListWidget::leaveEvent(event);
	//Q_EMIT lostFocus();
}

void NGui::ToDoListContent::addToDoListItem()
{
	NF::StickyNoteToDoListItem data;
	addToDoListItem(data);
}

void NGui::ToDoListContent::addToDoListItem(const QString& text)
{
	NF::StickyNoteToDoListItem data;
	data.contentText = text.toStdString();
	addToDoListItem(data);
}

int NGui::ToDoListContent::getToDoListCount()
{
	return d->ItemWidgetMap.size();
}

QSize NGui::ToDoListContent::getToDoListSize()
{
	if (d->ItemWidgetMap.size() == 0)
		return QSize(0, 0);
	return d->ItemWidgetMap.begin()->second->size();
}

QList<QString> NGui::ToDoListContent::getToDOListText()
{
	QList<QString> listStr;

	for (auto iter = d->ItemWidgetMap.begin(); iter != d->ItemWidgetMap.end(); iter++)
	{
		listStr.push_back(iter->second->getText());
	}

	return listStr;
}

void NGui::ToDoListContent::setSplit(const bool& b)
{
	if (d->isSplit == b)
		return;

	d->isSplit = b;

	if (d->isSplit)
		splitItem();
	else
		cancelSplitItem();
}

bool NGui::ToDoListContent::getSplit()
{
	return d->isSplit;
}

void NGui::ToDoListContent::addToDoListItem(NF::StickyNoteToDoListItem& data)
{
	auto item = new QListWidgetItem();
	auto itemWidget = new ToDoListItem();
	itemWidget->setData(data);
	item->setSizeHint(itemWidget->sizeHint());
	insertItem(row(d->addItem), item);
	setItemWidget(item, itemWidget);
	d->ItemWidgetMap[item] = itemWidget;

	connect(itemWidget, SIGNAL(deleteItem()), this, SLOT(deleteItem()));
	//绑定窗口焦点状态
	connect(itemWidget, &ToDoListItem::gotFocus, this, [&] {
		//Q_EMIT gotFocus();
		});
	connect(itemWidget, &ToDoListItem::lostFocus, this, [&] {
		//Q_EMIT lostFocus();
		});
}

void NGui::ToDoListContent::deleteItem()
{
	auto itemWidget = dynamic_cast<ToDoListItem*>(sender());

	for (auto iter = d->ItemWidgetMap.begin(); iter != d->ItemWidgetMap.end(); iter++)
	{
		if (iter->second != itemWidget)
			continue;
		this->takeItem(row(iter->first));
		delete iter->first;
		itemWidget->deleteLater();
		d->ItemWidgetMap.erase(iter);
		break;
	}
}

void NGui::ToDoListContent::addItemButtonClicked()
{
	addToDoListItem();
}

void NGui::ToDoListContent::splitItem()
{
	//先移除所有的项
	removeAllItem();
	//将未完成 和 已完成的事项分别建表排序
	std::list<QListWidgetItem*> finishedItems, notFinishedItems;

	for (auto iter = d->ItemWidgetMap.begin(); iter != d->ItemWidgetMap.end(); iter++)
	{
		if (iter->second->isFinished())
		{
			finishedItems.push_back(iter->first);
		}
		else {
			notFinishedItems.push_back(iter->first);
		}
	}

	//排序 
	finishedItems.sort([&](QListWidgetItem* item1, QListWidgetItem* item2) {
		if (d->ItemWidgetMap[item1]->getFinishedDateTime() == d->ItemWidgetMap[item2]->getFinishedDateTime())
			return false;
		else if (d->ItemWidgetMap[item1]->getFinishedDateTime() > d->ItemWidgetMap[item2]->getFinishedDateTime())
			return true;
		return false;
		});
	notFinishedItems.sort([&](QListWidgetItem* item1, QListWidgetItem* item2) {
		if (d->ItemWidgetMap[item1]->getFinishedDateTime() == d->ItemWidgetMap[item2]->getFinishedDateTime())
			return false;
		else if (d->ItemWidgetMap[item1]->getFinishedDateTime() > d->ItemWidgetMap[item2]->getFinishedDateTime())
			return true;
		return false;
		});

	auto pr = d->addItemWidget->parent();

	//依次添加固定的item 次序是
	//未完成 -》 添加  -》 已完成
	addItem(d->notFinishedSplitterItem);
	addItem(d->addItem);
	addItem(d->finishedSplitterItem);

	/*
	* 由于未知原因。addItemWidget会被qt析构掉，导致程序崩溃
	* 所以在这个地方手动释放内存，重新创建对象 
	*/

	delete d->addItemWidget;
	d->addItemWidget = new ToDoListAddItemWidget();
	d->addItem->setSizeHint(d->addItemWidget->sizeHint());

 	setItemWidget(d->addItem, d->addItemWidget);
	setItemWidget(d->finishedSplitterItem, d->finishedSplitterWidget);
	setItemWidget(d->notFinishedSplitterItem, d->notFinishedSplitterWidget);

	//遍历两个排序的表，然后插入各自的位置
	for (auto it : notFinishedItems)
	{
		insertItem(row(d->addItem), it);
		setItemWidget(it, d->ItemWidgetMap[it]);
	}

	for (auto it : finishedItems)
	{
		insertItem(row(d->finishedSplitterItem)+1, it);
		setItemWidget(it, d->ItemWidgetMap[it]);
	}


}

void NGui::ToDoListContent::cancelSplitItem()
{
	//移除所有项
	removeAllItem();

	//获取所有的事项
	//然后使用创建事件排序

	std::list<QListWidgetItem*> listItems;

	for (auto it : d->ItemWidgetMap)
	{
		listItems.push_back(it.first);
	}

	listItems.sort([&](QListWidgetItem* item1, QListWidgetItem* item2) {
		if (d->ItemWidgetMap[item1]->getCreatDateTime() == d->ItemWidgetMap[item2]->getCreatDateTime())
			return false;
		else if (d->ItemWidgetMap[item1]->getCreatDateTime() > d->ItemWidgetMap[item2]->getCreatDateTime())
			return true;
		return false;
		});

	/*
	* 由于未知原因。addItemWidget会被qt析构掉，导致程序崩溃
	* 所以在这个地方手动释放内存，重新创建对象
	*/

	delete d->addItemWidget;
	d->addItemWidget = new ToDoListAddItemWidget();
	d->addItem->setSizeHint(d->addItemWidget->sizeHint());

	//添加新增按钮
	addItem(d->addItem);
	setItemWidget(d->addItem, d->addItemWidget);

	//按照排序顺序写入
	for (auto it : listItems)
	{
		insertItem(row(d->addItem), it);
		setItemWidget(it, d->ItemWidgetMap[it]);
	}
}

void NGui::ToDoListContent::removeAllItem()
{
	for (auto it : d->ItemWidgetMap)
	{
		removeItemWidget(it.first);
	}
	while (count())
	{
		takeItem(0);
	}
}


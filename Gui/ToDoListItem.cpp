#include "ToDoListItem.h"
#include "ui_ToDoListItem.h"
#include "OriginUI/Action.h"
#include <QAction>
#include <QHBoxLayout>
#include <QDateTime>
#include <iostream>

namespace NGui {
	class ToDoListItemD {
	public:
		NF::StickyNoteToDoListItem itemData;
	};

}

NGui::ToDoListItem::ToDoListItem(QWidget* parent /*= 0*/)
	:QWidget(parent),ui(new Ui::ToDoListItem())
{
	ui->setupUi(this);
	d = new ToDoListItemD();

	OriginUI::Action* isFinishedAction = new OriginUI::Action();
	isFinishedAction->setParent(this);
	isFinishedAction->setCheckIcon(QIcon(":/icon/to_do_list_finished.svg"));
	isFinishedAction->setNoCheckIcon(QIcon(":/icon/to_do_list_unFinished.svg"));
	isFinishedAction->setToolTip(tr("Set to do list is finished!"));
	isFinishedAction->setCheckable(true);
	ui->toolButtonIsFInish->setDefaultAction(isFinishedAction);
	ui->toolButtonIsFInish->setIconSize(QSize(20, 20));

	ui->toolButtonDelete->setDefaultAction(new QAction(this));
	ui->toolButtonDelete->setIcon(QIcon(":/icon/to_do_list_delete.svg"));
	ui->toolButtonDelete->setToolTip(tr("Delete to do list!"));
	ui->toolButtonDelete->setIconSize(QSize(20, 20));

	ui->toolButtonDate->setDefaultAction(new QAction(this));
	ui->toolButtonDate->setIcon(QIcon(":/icon/to_do_list_plan.svg"));
	ui->toolButtonDate->setToolTip(tr("Set to do list plan"));
	ui->toolButtonDate->setIconSize(QSize(20, 20));

	hideEditButton();

	//发送输入框的焦点状态
	connect(ui->editLable, &OriginUI::EditLable::gotFocus, this, [&]() {
		Q_EMIT gotFocus();
		});
	connect(ui->editLable, &OriginUI::EditLable::lostFocus, this, [&]() {
		Q_EMIT lostFocus();
		});
}

NGui::ToDoListItem::~ToDoListItem()
{
	delete ui;
	delete d;
}

void NGui::ToDoListItem::on_toolButtonIsFInish_triggered(QAction* action)
{
	d->itemData.isFilished = ui->toolButtonIsFInish->isChecked();
	if (action->isChecked())
	{
		d->itemData.finishedDate = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss").toStdString();
	}else {
		d->itemData.finishedDate = "";
	}
	updateLableDate();
}

void NGui::ToDoListItem::on_toolButtonDelete_triggered(QAction* action)
{
	emit deleteItem();
}

void NGui::ToDoListItem::on_toolButtonDate_triggered(QAction* action)
{

}

void NGui::ToDoListItem::updateLableDate()
{
	QString dateLableText = QString::fromStdString(d->itemData.createDate.getValue());
	if (d->itemData.isFilished.getValue())
		dateLableText += "  -  " + QString::fromStdString(d->itemData.finishedDate.getValue());
	ui->labelDate->setText(dateLableText);
}

void NGui::ToDoListItem::setData(NF::StickyNoteToDoListItem data)
{
	ui->toolButtonIsFInish->setChecked(data.isFilished.getValue());
	ui->toolButtonIsFInish->defaultAction()->setChecked(data.isFilished.getValue());
	ui->editLable->setText(QString::fromStdString(data.contentText.getValue()));

	d->itemData = data;

	updateLableDate();
}

NF::StickyNoteToDoListItem NGui::ToDoListItem::getData()
{
	d->itemData.contentText = ui->editLable->getText().toStdString();
	return d->itemData;
}

QString NGui::ToDoListItem::getText() const
{
	return ui->editLable->getText();
}

void NGui::ToDoListItem::setText(const QString& text)
{
	ui->editLable->setText(text);
}

QDateTime NGui::ToDoListItem::getCreatDateTime()
{
	QDateTime date;
	date.fromString(QString::fromStdString(d->itemData.createDate.getValue()), "yyyy-MM-dd hh:mm:ss");
	return date;
}

QDateTime NGui::ToDoListItem::getFinishedDateTime()
{
	QDateTime date;
	date.fromString(QString::fromStdString(d->itemData.finishedDate .getValue()), "yyyy-MM-dd hh:mm:ss");
	return date;
}

bool NGui::ToDoListItem::isFinished()
{
	return d->itemData.isFilished.getValue();
}

void NGui::ToDoListItem::showEditButton()
{
	//ui->toolButtonDate->setHidden(false);
	ui->toolButtonDelete->setHidden(false);
}

void NGui::ToDoListItem::hideEditButton()
{
	ui->toolButtonDate->setHidden(true);
	ui->toolButtonDelete->setHidden(true);
}

NGui::ToDoListAddItemWidget::ToDoListAddItemWidget(QWidget* parent /*= 0*/)
	:QWidget(parent)
{
	setLayout(new QHBoxLayout(this));
	auto addButton = new QToolButton(this);
	QIcon icon(":/icon/to_do_list_new_item.svg");
	addButton->setIcon(icon);
	addButton->setIconSize(QSize(20, 20));
	addButton->setMinimumSize(40, 40);
	addButton->setToolTip(tr("Add to do list item!"));

	addButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	this->layout()->addWidget(addButton);
	layout()->setContentsMargins(0, 0,0 , 0);
	//layout()->setSpacing(0);
	connect(addButton, &QToolButton::clicked, [&]() {
		emit clicked();
		});
}

NGui::ToDoListAddItemWidget::~ToDoListAddItemWidget()
{

}

#include "EditLable.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QIcon>
#include <QToolButton>
#include "TempTextEdit.h"
#include "InputLineEdit.h"
namespace OriginUI {

	class EditLableD {
	public:
		EditLableD(EditLable* editLable);
		~EditLableD();
		ClickedLable *lable;
		InputLineEdit *lineEdit;
	};

	EditLableD::EditLableD(EditLable* editLable)
	{
		lable = new ClickedLable();
		lable->setText("Edit lable");
		lable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		lineEdit = new InputLineEdit();
		lineEdit->setOkIcon(QIcon(":/icon/ok.svg"));
		lineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		lineEdit->setAutoShowCancelButton(false);
		
		auto vLayout = new QVBoxLayout(editLable);
		vLayout->addWidget(lable);
		vLayout->setContentsMargins(0,0,0,0);
		vLayout->setSpacing(0);
		editLable->connect(lable, SIGNAL(mouseDoubleClick()), editLable, SLOT(lableDoubleClicked()));
		editLable->connect(lineEdit->getOkButton(), SIGNAL(clicked(bool)), editLable, SLOT(lineEditOk(bool)));
		editLable->connect(lineEdit->getLineEdit(), SIGNAL(editingFinished()), editLable, SLOT(editingFinished()));
	}

	EditLableD::~EditLableD()
	{
		delete lable;
		delete lineEdit;
	}

	ClickedLable::ClickedLable(QWidget* parent /*= 0*/)
		:QLabel(parent)
	{

	}

	void ClickedLable::mouseDoubleClickEvent(QMouseEvent* event)
	{
		QLabel::mouseDoubleClickEvent(event);
		Q_EMIT mouseDoubleClick();
	}

	LineEditLeave::LineEditLeave(QWidget* parent /*= 0*/)
		:QLineEdit(parent)
	{

	}

	void LineEditLeave::leaveEvent(QEvent* event)
	{
		QLineEdit::leaveEvent(event);
		emit mouseLeave();
	}

}



OriginUI::EditLable::EditLable(QWidget* parent /*= 0*/)
	:QWidget(parent)
{
	d = new EditLableD(this);
	d->lable->setToolTip(tr("double click edit"));

	connect(d->lineEdit, &InputLineEdit::gotFocus, this, [&]() {
		Q_EMIT gotFocus();
		});
	connect(d->lineEdit, &InputLineEdit::lostFocus, this, [&]() {
		Q_EMIT lostFocus();
		});

}

OriginUI::EditLable::~EditLable()
{
	delete d;
}

void OriginUI::EditLable::setText(const QString& text)
{
	d->lable->setText(text);
}

QString OriginUI::EditLable::getText() const
{
	return d->lable->text();
}

void OriginUI::EditLable::showLabel()
{
	layout()->removeWidget(d->lineEdit);
	d->lineEdit->hide();
	layout()->addWidget(d->lable);
	d->lable->show();
}

void OriginUI::EditLable::showLineEdit()
{
	layout()->removeWidget(d->lable);
	d->lable->hide();
	layout()->addWidget(d->lineEdit);
	d->lineEdit->show();
	d->lineEdit->getLineEdit()->setFocus();
}

void OriginUI::EditLable::lableDoubleClicked()
{
	d->lineEdit->getLineEdit()->setText(d->lable->text());
	showLineEdit();
}

void OriginUI::EditLable::editOkButtonClicked(bool)
{
	d->lineEdit->hide();
	d->lable->setText(d->lineEdit->getLineEdit()->text());
	showLabel();
}

void OriginUI::EditLable::editCancelButtonClicked(bool)
{
	showLabel();
}

void OriginUI::EditLable::lineEditOk(bool)
{
	d->lable->setText(d->lineEdit->getLineEdit()->text());
	showLabel();
}

void OriginUI::EditLable::editingFinished()
{
	lineEditOk(true);
}


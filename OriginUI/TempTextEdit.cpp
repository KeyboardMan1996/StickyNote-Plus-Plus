#include "TempTextEdit.h"
#include <QToolButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include "LogTool.h"
namespace OriginUI {
	class TempTextEditD {
	public:
		TempTextEditD(TempTextEdit* tempEdit);
		~TempTextEditD() = default;
		QLineEdit* textEdit;
		QToolButton* buttonOK, * buttonCancel;
	};

	TempTextEditD::TempTextEditD(TempTextEdit* tempEdit)
	{
		textEdit = new QLineEdit(tempEdit);
		QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
		sizePolicy.setHorizontalStretch(0);
		sizePolicy.setVerticalStretch(0);
		textEdit->setSizePolicy(sizePolicy);

		buttonOK = new QToolButton();
		buttonCancel = new QToolButton();

		auto hLayout = new QHBoxLayout();
		auto vLayout = new QVBoxLayout();

		hLayout->addWidget(buttonOK);
		hLayout->addWidget(buttonCancel);

		vLayout->setSpacing(10);
		vLayout->addWidget(textEdit);
		vLayout->addLayout(hLayout);
		vLayout->setStretch(10, 1);

		tempEdit->setLayout(vLayout);

		buttonOK->connect(buttonOK, SIGNAL(clicked(bool)), tempEdit, SLOT(toolButtonClicked(bool)));
		buttonOK->connect(buttonCancel, SIGNAL(clicked(bool)), tempEdit, SLOT(toolButtonClicked(bool)));
	}

}

OriginUI::TempTextEdit::TempTextEdit(QWidget* parent /*= 0*/)
{
	d = new TempTextEditD(this);
}

OriginUI::TempTextEdit::~TempTextEdit()
{
	delete d;
}

void OriginUI::TempTextEdit::setText(const QString& text)
{
	d->textEdit->setText(text);
}


QString OriginUI::TempTextEdit::getText() const
{
	return d->textEdit->text();
}

void OriginUI::TempTextEdit::toolButtonClicked(bool b)
{
	auto button = dynamic_cast<QToolButton*> (sender());
	if (PtrIsNull(button))
		return;
	if (button == d->buttonOK)
		Q_EMIT okButtonClicked(b);
	else if (button == d->buttonCancel)
		Q_EMIT cancelButtonClicked(b);
}

void OriginUI::TempTextEdit::leaveEvent(QEvent* event)
{
	QWidget::leaveEvent(event);
	//不能在鼠标离开时改变文本
	//因为傻狗输入法，输入时也会发出该事件 
/*	Q_EMIT okButtonClicked(true);*/
}


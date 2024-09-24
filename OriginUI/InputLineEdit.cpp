#include "InputLineEdit.h"
#include "LogTool.h"
#include <QHBoxLayout>
#include <QLineEdit>
#include <QToolButton>
#include <QStyleOption>
#include <QPainter>
#include <QKeyEvent>
namespace OriginUI {

	class InputLineEditEventFilter :public QObject {
	public:
		InputLineEditEventFilter(QObject* parent = 0):QObject(parent) {};
		~InputLineEditEventFilter() = default;
		InputLineEdit* Edit = nullptr;
	protected:
		bool eventFilter(QObject* watched, QEvent* event) {
			do 
			{
				if (event->type() != QEvent::KeyPress)
					break;
				auto e = dynamic_cast<QKeyEvent*>(event);
				if (e->key() != Qt::Key_Enter &&
					e->key() != Qt::Key_Return)
					break;
				if (PtrIsNull(Edit))
					break;
				Edit->getOkButton()->click();
				return true;
			} while (0);

			return QObject::eventFilter(watched, event);
		};
	};



	class InputLineEditD {
	public:
		InputLineEditD(InputLineEdit* widget);
		~InputLineEditD();
		QHBoxLayout* layout;
		InputLineEditFocus* lineEdit;
		QToolButton* okButton,*cancelButton;
		InputLineEditLayoutMod mod;
		//是否自动显示取消按钮
		bool autoShowCancelButton;
	};

	InputLineEditD::InputLineEditD(InputLineEdit *widget)
		:mod(BUTTON_RIGHT),autoShowCancelButton(true)
	{
		layout = new QHBoxLayout;
 		layout->setSpacing(0);
 		layout->setContentsMargins(0,0,0,0);
		

		lineEdit = new InputLineEditFocus();
		//为编辑器添加一个事件过滤器，截获回车按下事件
		auto filter = new InputLineEditEventFilter(lineEdit);
		filter->Edit = widget;
		lineEdit->installEventFilter(filter);

		QSizePolicy policy;
		policy.setVerticalPolicy(QSizePolicy::Policy::Expanding);
		policy.setHorizontalPolicy(QSizePolicy::Policy::Preferred);
		lineEdit->setSizePolicy(policy);
		widget->connect(lineEdit, SIGNAL(textChanged(QString)), widget, SLOT(textChanged(QString)));
		layout->addWidget(lineEdit);

		cancelButton = new QToolButton();
		cancelButton->setMinimumSize(32, 32);
		cancelButton->hide();
		layout->addWidget(cancelButton);

		okButton = new QToolButton();
		okButton->setMinimumSize(32, 32);
		layout->addWidget(okButton);

		widget->setLayout(layout);
	}

	InputLineEditD::~InputLineEditD()
	{
		layout->deleteLater();
		okButton->deleteLater();
		cancelButton->deleteLater();
		lineEdit->deleteLater();
	}

	void InputLineEditFocus::focusInEvent(QFocusEvent* e)
	{
		QLineEdit::focusInEvent(e);
		if (e->gotFocus())
			Q_EMIT gotFocus();
	}

	void InputLineEditFocus::focusOutEvent(QFocusEvent* e)
	{
		QLineEdit::focusOutEvent(e);
		if (e->lostFocus())
			Q_EMIT lostFocus();
	}

}


OriginUI::InputLineEdit::InputLineEdit(QWidget* parent /*= 0*/)
	:QWidget(parent)
{
	d = new InputLineEditD(this);

	//发送编辑器的焦点状态
	connect(d->lineEdit, &InputLineEditFocus::gotFocus, this, [&]() {
		Q_EMIT gotFocus();
		});
	connect(d->lineEdit, &InputLineEditFocus::lostFocus, this, [&]() {
		Q_EMIT lostFocus();
		});


}

OriginUI::InputLineEdit::~InputLineEdit()
{
	delete d;
}

void OriginUI::InputLineEdit::setButtonLaoutMod(const InputLineEditLayoutMod& mod)
{
	if (d->mod == mod)
		return;

	if (mod == BUTTON_RIGHT)
	{
		d->layout->removeWidget(d->okButton);
		d->layout->removeWidget(d->cancelButton);
		d->layout->addWidget(d->cancelButton);
		d->layout->addWidget(d->okButton);
	}else if (mod == BUTTON_LEFT)
	{
		d->layout->removeWidget(d->lineEdit);
		d->layout->removeWidget(d->okButton);
		d->layout->removeWidget(d->cancelButton);
		d->layout->addWidget(d->okButton);
		d->layout->addWidget(d->cancelButton);
		d->layout->addWidget(d->lineEdit);
	}

	d->mod = mod;
}

OriginUI::InputLineEditLayoutMod OriginUI::InputLineEdit::getButtonLaoutMod()
{
	return d->mod;
}

void OriginUI::InputLineEdit::setOkIcon(QIcon icon)
{
	d->okButton->setIcon(icon);
}

void OriginUI::InputLineEdit::setCancelIcon(QIcon icon)
{
	d->cancelButton->setIcon(icon);
}

QToolButton* OriginUI::InputLineEdit::getOkButton()
{
	return d->okButton;
}

QToolButton* OriginUI::InputLineEdit::getCancelButton()
{
	return d->cancelButton;
}

QLineEdit* OriginUI::InputLineEdit::getLineEdit()
 {
	return d->lineEdit;
 }

void OriginUI::InputLineEdit::setAutoShowCancelButton(const bool& b)
{
	d->autoShowCancelButton = b;
	if(d->autoShowCancelButton)
		connect(d->lineEdit, SIGNAL(textChanged(QString)), this, SLOT(textChanged(QString)));
	else
		disconnect(d->lineEdit, SIGNAL(textChanged(QString)), this, SLOT(textChanged(QString)));
}

bool OriginUI::InputLineEdit::getAutoShowCancelButton()
{
	return d->autoShowCancelButton;
}

void OriginUI::InputLineEdit::textChanged(const QString& text)
{
	if (text.isEmpty())
		d->cancelButton->hide();
	else
		d->cancelButton->show();
}

void OriginUI::InputLineEdit::paintEvent(QPaintEvent* event)
{
	QStyleOption opt;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	opt.initFrom(this);
#else
	opt.init(this);
#endif    
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void OriginUI::InputLineEdit::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Enter)
	{
		d->okButton->toggled(true);
		return;
	}
	QWidget::keyReleaseEvent(event);
}


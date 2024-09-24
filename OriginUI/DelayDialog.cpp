#include "DelayDialog.h"
#include "ui_DelayDialog.h"

namespace OriginUI {
	class DelayDialogD {
	public:
		DelayDialogD() {
			time = 15;
			okButtonText = "OK";
			cancelButtonText = "Cancel";
			timerID = 0;
			result = DelayDialog::CANCEL;
		}
	public:
		int  time;
		QString okButtonText;
		QString cancelButtonText;
		int timerID;
		DelayDialog::Result result;
	};
}

OriginUI::DelayDialog::DelayDialog(QWidget* parent /*= 0*/)
	:QDialog(parent),ui(new Ui::DelayDialog())
{
	ui->setupUi(this);
	d = new DelayDialogD();

	//设置按钮延时
	ui->pushButtonOk->setEnabled(false);
	d->timerID = startTimer(1000);

	//绑定按钮槽函数
	connect(ui->pushButtonOk, &QPushButton::clicked, this, &OriginUI::DelayDialog::okButtonClicked);
	connect(ui->pushButtonCancel, &QPushButton::clicked, this, &OriginUI::DelayDialog::cancelButtonClicked);

}

OriginUI::DelayDialog::~DelayDialog()
{

}

void OriginUI::DelayDialog::setOkButtonText(const QString& text)
{
	d->okButtonText = text + "(%1)";
	ui->pushButtonOk->setText(d->okButtonText.arg(d->time));
}

void OriginUI::DelayDialog::setCancelbuttonText(const QString& text)
{
	d->cancelButtonText = text;
	ui->pushButtonCancel->setText(text);
}

void OriginUI::DelayDialog::setContentText(const QString& text)
{
	ui->label->setText(text);
}

OriginUI::DelayDialog::Result OriginUI::DelayDialog::getResult()
{
	return d->result;
}

void OriginUI::DelayDialog::setDelayTime(const int sec)
{
	d->time = sec;
}

void OriginUI::DelayDialog::timerEvent(QTimerEvent* event)
{
	if (d->time == 0)
	{
		ui->pushButtonOk->setEnabled(true);
		killTimer(d->timerID);
	}else {
		d->time--;
		ui->pushButtonOk->setText(d->okButtonText.arg(d->time));
	}

	QDialog::timerEvent(event);
}

void OriginUI::DelayDialog::okButtonClicked(bool)
{
	d->result = DelayDialog::OK;
	close();
}

void OriginUI::DelayDialog::cancelButtonClicked(bool)
{
	d->result = DelayDialog::CANCEL;
	close();
}


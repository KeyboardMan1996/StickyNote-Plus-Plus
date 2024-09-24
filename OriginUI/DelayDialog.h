#pragma  once
#include <QDialog>
#include "ExportConfig.hpp"
namespace Ui {
	class DelayDialog;
}
namespace OriginUI {
	class DelayDialogD;
	class ORIGIN_UI_EXPORT DelayDialog :public QDialog {
		Q_OBJECT
	public:
		enum Result{
			OK = 0,
			CANCEL = 1
		};
	public:
		DelayDialog(QWidget *parent = 0);
		~DelayDialog();

	public:
		void setOkButtonText(const QString& text);
		void setCancelbuttonText(const QString& text);
		void setContentText(const QString& text);
		Result getResult();
		void result() = delete;
		void setDelayTime(const int sec);
	protected:
		void timerEvent(QTimerEvent* event);

	public slots:
		void okButtonClicked(bool);
		void cancelButtonClicked(bool);
	private:
		Ui::DelayDialog* ui;
		DelayDialogD* d;
	};
}
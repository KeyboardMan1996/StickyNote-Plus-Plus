#pragma once
#include "WindowNoBoder.h"
namespace Ui {
	class UpdateUI;
}
namespace NGui {
	class UpdateUI :public OriginUI::WindowNoBoder {
	public:
		UpdateUI(QWidget* parent = 0);
		~UpdateUI();
	public:
		void setUrl(const QUrl& url);
		void setNewVersion(const double& version);
		void setContent(const QString& content);
	public slots:
		void updateButtonClicked();
		void ignoreButtonClicked();
	private:
		Ui::UpdateUI* ui;
		//链接
		QUrl url;
		//最新版本
		double newVersion;
	};
}
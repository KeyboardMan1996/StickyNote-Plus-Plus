#pragma once
#include "ExportConfig.hpp"
#include <QToolButton>
#include <QColor>
#include <QPaintEvent>

namespace OriginUI {

	class ToolButtonColorD;
	class ORIGIN_UI_EXPORT ToolButtonColor :public QToolButton{
		Q_OBJECT
	public:
		ToolButtonColor(QWidget* parent = 0);
		~ToolButtonColor();

	public:
		void setColor(const QColor& c);
		QColor getColor();

		void setSettingButtonEnable(const bool& b);
		bool getSettingButtonEnable();

		void setIcon(const QIcon& icon);
	public Q_SLOTS:
		void settingButtonClicked(bool);

	protected:
		void paintEvent(QPaintEvent* event);
		void checkStateSet();
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
		void enterEvent(QEnterEvent* event) override;
#else
		void enterEvent(QEvent* event) override;
#endif
		void leaveEvent(QEvent* event);
		void resizeEvent(QResizeEvent* event);

	private:
		void addSettingButton();
		void removeSettingButton();
		void autoSettingButtonSize();
		void autoSettingButtonPos();
		void autoSettingState();
	private:
		ToolButtonColorD* d;
	};

}
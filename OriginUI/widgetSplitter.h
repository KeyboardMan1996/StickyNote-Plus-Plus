#pragma  once
#include <QWidget>
#include "ExportConfig.hpp"
namespace OriginUI {

	class LineWidgetD;
	class ORIGIN_UI_EXPORT LineWidget :public QWidget{
	public:
		LineWidget(QWidget* parent = 0);
		~LineWidget();

		void setColor(const QColor& color);
		QColor getColor();

	protected:
		void paintEvent(QPaintEvent* event);
	private:
		LineWidgetD* d;
	};

	class WidgetSplitterD;
	class ORIGIN_UI_EXPORT WidgetSplitter :public QWidget{
	public:
		WidgetSplitter(QWidget* parent = 0);
		~WidgetSplitter();

		void setFontColor(const QColor& color);
		QColor getFontColor();

		void setLineColor(const QColor& color);
		QColor getLineColor();

		void setText(const QString& text);
		QString getText();

		void setLineHeight(const int& height);
		int getLineHeight();
	private:
		void initGui();
	private:
		WidgetSplitterD* d;

	};

}
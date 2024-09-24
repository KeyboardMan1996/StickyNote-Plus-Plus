#pragma once
#include "ExportConfig.hpp"
#include <QTextEdit>


namespace OriginUI {
	class  SimpleRichTextEditD;
	class ORIGIN_UI_EXPORT SimpleRichTextEdit :public QTextEdit{
		Q_OBJECT
	public:
		SimpleRichTextEdit(QWidget* parent = 0);
		~SimpleRichTextEdit();

	public:
		//调大字体
		void fontSizeUp();
		//调小字体
		void fontSizeDown();
		//设置下滑线
		void setUnderLine(const bool& b);
		bool getUnderLine();
		//设置删除线
		void setStrikOut(const bool& b);
		bool getStrikOut();
		//设置居中
		void setTextCenter(const bool& b);
		bool getTextCenter();
		//设置居左
		void setTextLeft();
		bool getTextLeft();
		//设置居右
		void setTextRight(const bool& b);
		bool getTextRight();
		//设置当前文本为list
		void setListFormat(QString format = "1. ");
		bool getListFormat();
		//设置自动识别列表
		void setAutoList(const bool& b);

		//设置字体颜色
		void setFontColor(const QColor& color);
		QColor getFontColor();

		//设置段落背景色
		void setBackground(const QColor& color);
		//清空段落背景色
		void clearBackground();
		//获取段落背景色
		QColor getBackground();
		//设置是否在点击URL自动跳转到链接
		void setAutoOpenUrl(const bool& b);
		bool getAutoOpenUrl();
		//设置斜体
		void setItalic(const bool& b);
		bool getItalic();
		//设置粗体
		void setFontBlod(const bool& b);
		bool getFontBlod();
		
		//获取当前块文本格式，或者获取当前选中的文本格式
		QTextCharFormat getTextCharFormat();

	protected:
		void insertFromMimeData(const QMimeData* source) override;
	public Q_SLOTS:
		void inserChanged(int position, int charsRemoved, int charsAdded);
	private:
		void setFormatOnWordOrSelection(const QTextCharFormat& format);
		void mergeFormatOnWordOrSelection(const QTextCharFormat& format);
		QTextCharFormat getFormatOnWordOrSelection();
		void openUrlFromCurrentCursor();

	protected:
		void mousePressEvent(QMouseEvent* e) override;
		void keyPressEvent(QKeyEvent* event) override;
	private:
		SimpleRichTextEditD* d;
	};

}
#include "SimpleRichTextEdit.h"
#include <QTextCharFormat>
#include <QTextListFormat>
#include <QTextCursor>
#include <QTextBlock>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <iostream>
#include <QUrl>
#include <QDesktopServices>
#include <QTextDocumentFragment>
#include <QMimeData>
namespace OriginUI {
	class SimpleRichTextEditD {
	public:
		SimpleRichTextEditD();
		~SimpleRichTextEditD();
	public:
		//自动识别列表正则
		QRegularExpression * listHeadRex;
		//自动识别URL正则
		QRegularExpression *urlRex;
		//是否自动打开url
		bool autoOpenUrl;
	};

	SimpleRichTextEditD::SimpleRichTextEditD()
	{
		listHeadRex = new QRegularExpression("^(1|a|A)\\.\\s");
		urlRex = new QRegularExpression("(http|ftp|https):\\/\\/[\\w\\-_]+(\\.[\\w\\-_]+)+([\\w\\-\\.,@?^=%&:/~\\+#]*[\\w\\-\\@?^=%&/~\\+#])?");
		autoOpenUrl = true;
	}

	SimpleRichTextEditD::~SimpleRichTextEditD()
	{
		delete listHeadRex;
		delete urlRex;
	}

}

OriginUI::SimpleRichTextEdit::SimpleRichTextEdit(QWidget* parent /*= 0*/)
{
	d = new SimpleRichTextEditD();
}

OriginUI::SimpleRichTextEdit::~SimpleRichTextEdit()
{

}

void OriginUI::SimpleRichTextEdit::fontSizeUp()
{
	auto format = getFormatOnWordOrSelection();
	auto size = format.fontPointSize();
	if (format.isEmpty())
	{
		size = 13;
		QTextCharFormat nft;
		format = nft;
	}
	size++;
	format.setFontPointSize(size);
	mergeFormatOnWordOrSelection(format);
}

void OriginUI::SimpleRichTextEdit::fontSizeDown()
{
	auto format = getFormatOnWordOrSelection();
	auto size = format.fontPointSize();
	if (format.isEmpty())
	{
		size = 13;
		QTextCharFormat nft;
		format = nft;
	}
	size--;
	format.setFontPointSize(size);
	mergeFormatOnWordOrSelection(format);
}

void OriginUI::SimpleRichTextEdit::setUnderLine(const bool& b)
{
	QTextCharFormat format;
	format.setFontUnderline(b);
	mergeFormatOnWordOrSelection(format);
}


bool OriginUI::SimpleRichTextEdit::getUnderLine()
{
	QTextCharFormat format = getFormatOnWordOrSelection();
	return format.fontUnderline();
}

void OriginUI::SimpleRichTextEdit::setStrikOut(const bool& b)
{
	QTextCharFormat format;
	format.setFontStrikeOut(b);
	mergeFormatOnWordOrSelection(format);
}

bool OriginUI::SimpleRichTextEdit::getStrikOut()
{
	auto format = getTextCharFormat();

	return format.fontStrikeOut();
}

void OriginUI::SimpleRichTextEdit::setTextCenter(const bool& b)
{
	if (b)
		this->setAlignment(Qt::AlignHCenter);
	else
		this->setAlignment(Qt::AlignLeft);
}

bool OriginUI::SimpleRichTextEdit::getTextCenter()
{
	if ((this->alignment() & Qt::AlignHCenter) == Qt::AlignHCenter)
		return true;
	else
		return false;
}

void OriginUI::SimpleRichTextEdit::setTextLeft()
{
		this->setAlignment(Qt::AlignLeft);
}

bool OriginUI::SimpleRichTextEdit::getTextLeft()
{
	if ((this->alignment() & Qt::AlignLeft) == Qt::AlignLeft)
		return true;
	else
		return false;
}

void OriginUI::SimpleRichTextEdit::setTextRight(const bool& b)
{
	if (b)
		this->setAlignment(Qt::AlignRight);
	else
		this->setAlignment(Qt::AlignLeft);
}

bool OriginUI::SimpleRichTextEdit::getTextRight()
{
	if ((this->alignment() & Qt::AlignRight) == Qt::AlignRight)
		return true;
	else
		return false;
}

void OriginUI::SimpleRichTextEdit::setListFormat(QString format /*= "1. "*/)
{
	QTextListFormat listFormat;

	if (format == "1. ")
		listFormat.setStyle(QTextListFormat::ListDecimal);
	else if(format == "a. ")
		listFormat.setStyle(QTextListFormat::ListLowerAlpha);
	else if(format == "A. ")
		listFormat.setStyle(QTextListFormat::ListUpperAlpha);
// 	listFormat.setNumberPrefix("[");
// 	listFormat.setNumberSuffix("]");
	textCursor().insertList(listFormat);
}

bool OriginUI::SimpleRichTextEdit::getListFormat()
{
 	auto format = getFormatOnWordOrSelection();
// 	auto listFormat = format.toListFormat();
// 	if(listFormat.style() != QTextListFormat::ListStyleUndefined)
// 		return true;
// 	return false;

	if (format.type() == 3)
		return true;
	return false;
}

void OriginUI::SimpleRichTextEdit::setAutoList(const bool& b)
{
	if (b)
	{
		connect(this->document(), SIGNAL(contentsChange(int, int, int)),this,SLOT(inserChanged(int, int, int)));
	}else {
		disconnect(this->document(), SIGNAL(contentsChange(int, int, int)),this,SLOT(inserChanged(int, int, int)));
	}
}

void OriginUI::SimpleRichTextEdit::setFontColor(const QColor& color)
{
	QTextCharFormat format;
	format.setForeground(color);
	mergeFormatOnWordOrSelection(format);
}

QColor OriginUI::SimpleRichTextEdit::getFontColor()
{
	auto format = getFormatOnWordOrSelection();
	auto brush = format.foreground();
	return brush.color();
}

void OriginUI::SimpleRichTextEdit::setBackground(const QColor& color)
{
	QTextCharFormat format;
	format.setBackground(color);
	mergeFormatOnWordOrSelection(format);
}

void OriginUI::SimpleRichTextEdit::clearBackground()
{
	auto format = getFormatOnWordOrSelection();
	format.clearBackground();
	setFormatOnWordOrSelection(format);
}

QColor OriginUI::SimpleRichTextEdit::getBackground()
{
	auto format = getFormatOnWordOrSelection();
	return format.background().color();
}


void OriginUI::SimpleRichTextEdit::setAutoOpenUrl(const bool& b)
{
	d->autoOpenUrl = b;
}

bool OriginUI::SimpleRichTextEdit::getAutoOpenUrl()
{
	return d->autoOpenUrl;
}

void OriginUI::SimpleRichTextEdit::setItalic(const bool& b)
{
	QTextCharFormat format;
	format.setFontItalic(b);
	mergeFormatOnWordOrSelection(format);
}

bool OriginUI::SimpleRichTextEdit::getItalic()
{
	auto format = getFormatOnWordOrSelection();
	return format.fontItalic();
}

void OriginUI::SimpleRichTextEdit::setFontBlod(const bool& b)
{
	QTextCharFormat format;
	if (b)
		format.setFontWeight(QFont::Bold);
	else
		format.setFontWeight(QFont::Normal);
	mergeFormatOnWordOrSelection(format);
}

bool OriginUI::SimpleRichTextEdit::getFontBlod()
{
	auto format = getFormatOnWordOrSelection();
	if (format.fontWeight() == QFont::Bold)
		return true;
	else
		return false;
}

QTextCharFormat OriginUI::SimpleRichTextEdit::getTextCharFormat()
{
	return getFormatOnWordOrSelection();
}

/**
* 使得在粘贴文本时，不粘贴富文本格式
* @brief OriginUI::SimpleRichTextEdit::insertFromMimeData
* @param const QMimeData * source
* @return void
*/
void OriginUI::SimpleRichTextEdit::insertFromMimeData(const QMimeData* source)
{
	if (source->hasText())
	{
		QTextDocument doc;
		doc.setHtml(source->text());
		QTextCursor cu = textCursor();
		cu.insertText(doc.toPlainText());
		return;
	}
	QTextEdit::insertFromMimeData(source);
}

void OriginUI::SimpleRichTextEdit::inserChanged(int position, int charsRemoved, int charsAdded)
{
	QTextCursor cursor = this->textCursor();        //获取当前文本光标
	QString blockText = cursor.block().text();      //获取光标所在文本块的文本
	blockText = blockText.left(cursor.positionInBlock());   //获取光标之前的文本

	auto match = d->listHeadRex->match(blockText);
	if (!match.hasMatch())
		return;
	cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, blockText.size());
	setTextCursor(cursor);
	setListFormat(blockText);

}

/**
* 设置格式到当前选择的文本，如果无选择文本，则将格式应用于文本块
* @brief OriginUI::SimpleRichTextEdit::setFormatOnWordOrSelection
* @param const QTextCharFormat & format
* @return void
*/
void OriginUI::SimpleRichTextEdit::setFormatOnWordOrSelection(const QTextCharFormat& format)
{
	QTextCursor cursor = this->textCursor();
	//记录光标位置，在取消选择后位置会被移到最后
	auto position = cursor.position();
	bool hasSelection = cursor.hasSelection();
	if (!hasSelection)
	{
		cursor.select(QTextCursor::WordUnderCursor);
	}
	cursor.setCharFormat(format);
	this->setCurrentCharFormat(format);

	//如果之前没有选中，那么清除选中
	if (!hasSelection)
	{
		cursor.clearSelection();
		cursor.setPosition(position);
	}
	setTextCursor(cursor);
}

/**
* @brief OriginUI::SimpleRichTextEdit::mergeFormatOnWordOrSelection 为选中的字符调整格式 没有选中的字体则调整当前格式
* @param const QTextCharFormat & format
* @return void
*/
void OriginUI::SimpleRichTextEdit::mergeFormatOnWordOrSelection(const QTextCharFormat& format)
{
	QTextCursor cursor = this->textCursor();
	//记录光标位置，在取消选择后位置会被移到最后
	auto position = cursor.position();
	bool hasSelection = cursor.hasSelection();
	if (!hasSelection)
	{
		cursor.select(QTextCursor::WordUnderCursor);
	}
	cursor.mergeCharFormat(format);
	this->mergeCurrentCharFormat(format);
	//如果之前没有选中，那么清除选中
	if (!hasSelection)
	{
		cursor.clearSelection();
		cursor.setPosition(position);
	}
	setTextCursor(cursor);
}

QTextCharFormat OriginUI::SimpleRichTextEdit::getFormatOnWordOrSelection()
{
	QTextCursor cursor = this->textCursor();
	if (cursor.hasSelection())
	{
		return cursor.charFormat();
	}

	return currentCharFormat();
}

void OriginUI::SimpleRichTextEdit::openUrlFromCurrentCursor()
{
#if 0   //暂时不想要这个功能
	/*
*	如果光标在点击在连接中，那么直接用浏览器打开连接。
*	思路： 鼠标左键点击时会触发光标移动，当光标移动时
*	获取光标当前所在的文本块，匹配文本块中是否有网址，如果有，
*	那么获取网址在字符串中的位置，如果光标在网址位置中，那么打开网址。
*
*/
	if (!d->autoOpenUrl)
		return;

	//获取光标所在的位置
	QTextCursor cursor = textCursor();
	QString str = cursor.block().text();
	int blockPos = cursor.position() - cursor.block().position();

	//匹配正则，如果光标所在位置是一个链接,那么直接打开
	int pos = 0;
	int length = 0;

	while ((pos = d->urlRex.indexIn(str, pos)) != -1) {
		length = d->urlRex.matchedLength();
		if (blockPos - pos < length)
		{
			auto temp = str.mid(pos, length - 1);
			QUrl url(temp);
			QDesktopServices::openUrl(QUrl(url));
			break;
		}
		pos += d->urlRex.matchedLength();
	}
#endif

}

void OriginUI::SimpleRichTextEdit::mousePressEvent(QMouseEvent* e)
{

	QTextEdit::mousePressEvent(e);

	if (e->button() == Qt::LeftButton)
		openUrlFromCurrentCursor();

}

void OriginUI::SimpleRichTextEdit::keyPressEvent(QKeyEvent* event)
{
 	if (event->key() == Qt::Key_Space) {
		// 当按下空格键时，插入 `&nbsp;`
		this->insertHtml("&nbsp;");
	}
	else if (event->key() == Qt::Key_Tab) {
		//将tab键更换为4个空格
		this->insertHtml("&nbsp;&nbsp;&nbsp;&nbsp;");
	}
	else 
	{
		// 对于其他键的事件，使用默认的处理方式
		QTextEdit::keyPressEvent(event);
	}
}
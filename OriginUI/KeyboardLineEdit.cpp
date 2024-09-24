#include "KeyboardLineEdit.h"
#include "KeyboardToString.hpp"
#include <list>
#include <map>


namespace OriginUI {
	class KeyboardLineEditD {
	public:
		std::list<int> keys;
		bool keyboardRelease = true;
	};
}

OriginUI::KeyboardLineEdit::KeyboardLineEdit(QWidget* parent /*= 0*/)
	:QLineEdit(parent)
{
	d = new KeyboardLineEditD();
}

OriginUI::KeyboardLineEdit::~KeyboardLineEdit()
{
	delete d;
}

void OriginUI::KeyboardLineEdit::keyPressEvent(QKeyEvent* event)
{
	//���֮ǰ�Ѿ��а����ɿ�����ô�������밴��ʱ����Ҫ���֮ǰ�İ���
	if (d->keyboardRelease)
	{
		d->keys.clear();
		d->keyboardRelease = false;
	}
	d->keys.push_back(event->key());
	changeText();
}

void OriginUI::KeyboardLineEdit::keyReleaseEvent(QKeyEvent* event)
{
	d->keyboardRelease = true;
}

void OriginUI::KeyboardLineEdit::changeText()
{
	if (d->keys.size() == 0)
	{
		setText(" ");
		return;
	}
	auto iter = d->keys.begin();
	QString text = QString::fromStdString(KeyToString.at(*iter));
	iter++;
	for(;iter != d->keys.end();iter++)
		text += " + " + QString::fromStdString(KeyToString.at(*iter));
	setText(text);
}

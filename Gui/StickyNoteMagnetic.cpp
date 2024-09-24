#include "StickyNoteMagnetic.h"
#include "StickyNote.h"
#include <map>
#include <list>
#include <QRect>
#include <QPropertyAnimation>
NGui::StickyNoteMagnetic::StickyNoteMagnetic()
	:threshold(40),enable(true),space(5)
{

}

NGui::StickyNoteMagnetic::~StickyNoteMagnetic()
{

}

/**
* 窗口吸附判断，
* @brief NGui::StickyNoteMagnetic::magnetic
* @param StickyNote * note 移动位置的窗口
* @param std::vector<StickyNote * > pool 所有的便笺窗口
* @return void
*/
void NGui::StickyNoteMagnetic::magnetic(StickyNote* note, std::vector<StickyNote*> pool)
{
	/*
		[1] 得出该窗口与其他窗口之间的距离，这个距离只有满足阈值条件的距离为有效距离，否则均为无效距离
		[2] 记录下x轴或者y轴中的最远的距离
		[3] 得出距离最近的窗口
		[4] 如果没有满足阈值的窗口，则不触发吸附
		[5] 使用距离最近的窗口触发吸附
	*/

	//如果为不可用状态，不做任何操作
	if (!enable)
		return;
	//获取窗口位置大小信息
	StickyNoteRect rect(note);

	std::list<StickyNotePositon> postions;
	for (int i = 0; i < pool.size(); i++)
	{
		StickyNote* poolNote = pool[i];
		StickyNoteRect poolNoteRect(poolNote);
		//如果是同一个窗口，设置一个无效距离
		if (note == poolNote)
			continue;
		//如果重叠，不计算距离
		if(hasOverlap(note, poolNote))
			continue;
		
		double distanceX = (note->x() > poolNote->x()) ? note->x() - (poolNote->x() + poolNote->width()) : poolNote->x() - (note->x() + note->width());
		double distanceY = (note->y() > poolNote->y()) ? note->y() - (poolNote->y() + poolNote->height()) : poolNote->y() - (note->y() + note->height());

		//解决距离单方向距离为负，结果再算综合距离时，负数太大，会增大距离
		//这里将为负的距离重置为0
		distanceX = distanceX < 0 ? 0 : distanceX;
		distanceY = distanceY < 0 ? 0 : distanceY;


		//如果距离小于阈值，不触发
		if(distanceX > threshold || distanceY > threshold)
			continue;

		StickyNotePositon notePos;
		notePos.ditance = sqrt(distanceX * distanceX + distanceY * distanceY);
		notePos.rect = poolNoteRect;


		//判断哪个轴的距离比较远，就以哪个轴为基准
		//比如以x轴为基准，那么方向只能是左右
		if (distanceX > distanceY)
		{
			//以x轴为基准
			notePos.direction = note->x() < poolNote->x() ? LEFT : RIGHT;
		}else {
			//以y轴为基准
			notePos.direction = note->y() < poolNote->y() ? UP : DOWN;
		}
		postions.push_back(notePos);
	}

	if(postions.size() <= 0)
		return;
	postions.sort();

	for (auto postion : postions)
	{
		//判断移动后的位置是否与原先的窗口重复
		//如果重复，选择下一个窗口吸附
		StickyNoteRect r = moveResult(rect, postion);
		bool ok = false;
		for (auto iter = pool.begin(); iter != pool.end(); iter++)
		{
			//如果是自己，不判断
			if(*iter == note)
				continue;
			StickyNoteRect temp(*iter);
			ok = ok || hasOverlap(r, temp);
		}
		//如果重叠,选择下一个
		if(ok)
			continue;
		//如果位置超出屏幕外，则不触发
		if(r.x < 0 || r.y < 0)
			continue;
		startAnimation(note, r);
		break;
	}
}

/**
* 启动note1 向note2 移动的动画
* @brief NGui::StickyNoteMagnetic::startAnimation
* @param StickyNote * note1
* @param StickyNote * note2
* @return void
*/
void NGui::StickyNoteMagnetic::startAnimation(StickyNote* note1, StickyNote* note2)
{
	const int space = 2;
	QPropertyAnimation* animation = new QPropertyAnimation();
	QPoint pos1 = note1->pos();
	QPoint pos2 = note2->pos();

	QPoint pos;
	pos.setX(pos2.x() + note2->width() + space);
	pos.setY(pos2.y());

	animation->setPropertyName("pos");
	animation->setStartValue(pos1);
	animation->setEndValue(pos);
	animation->setTargetObject(note1);
	animation->setDuration(200);
	animation->start();

}

void NGui::StickyNoteMagnetic::setThreshold(const int& v)
{
	threshold = v;
}

int NGui::StickyNoteMagnetic::getThreshold()
{
	return threshold;
}

void NGui::StickyNoteMagnetic::setEnable(const bool& e)
{
	this->enable = e;
}

bool NGui::StickyNoteMagnetic::getEnable()
{
	return enable;
}

void NGui::StickyNoteMagnetic::setSpace(const int& s)
{
	this->space = s;
}

int NGui::StickyNoteMagnetic::getSpace()
{
	return space;
}

void NGui::StickyNoteMagnetic::startAnimation(StickyNote* note, const StickyNoteRect& rect)
{
	QPropertyAnimation* animation = new QPropertyAnimation();
	QPoint pos1 = note->pos();
	QPoint pos2 = note->pos();

	pos2.setX(rect.x);
	pos2.setY(rect.y);

	animation->setPropertyName("pos");
	animation->setStartValue(pos1);
	animation->setEndValue(pos2);
	animation->setTargetObject(note);
	animation->setDuration(200);
	animation->start();
}

bool NGui::StickyNoteMagnetic::hasOverlap(StickyNote* note1, StickyNote* note2)
{
	StickyNoteRect rc1(note1);
	StickyNoteRect rc2(note2);
	return hasOverlap(rc1, rc2);
}



NGui::StickyNoteRect NGui::StickyNoteMagnetic::moveResult(const StickyNoteRect& rect, const StickyNotePositon& magneticPos)
{
	StickyNoteRect result = rect;
	switch (magneticPos.direction)
	{
	case LEFT:
		result.x = magneticPos.rect.x - space - rect.width;
		result.y = magneticPos.rect.y;
		break;
	case RIGHT:
		result.x = magneticPos.rect.x + magneticPos.rect.width + space;
		result.y = magneticPos.rect.y;
		break;
	case UP:
		result.y = magneticPos.rect.y - result.height - space;
		result.x = magneticPos.rect.x;
		break;
	case DOWN:
		result.y = magneticPos.rect.height + magneticPos.rect.y + space;
		result.x = magneticPos.rect.x;
		break;
	}

	return result;
}

bool NGui::StickyNoteMagnetic::hasOverlap(const StickyNoteRect& rc1, const StickyNoteRect& rc2)
{
	if (rc1.x + rc1.width > rc2.x &&
		rc2.x + rc2.width > rc1.x &&
		rc1.y + rc1.height > rc2.y &&
		rc2.y + rc2.height > rc1.y
		)
		return true;
	else
		return false;

}


NGui::StickyNotePositon::StickyNotePositon()
	:ditance(0.0),direction(UP)
{

}

bool NGui::StickyNotePositon::operator>(const StickyNotePositon& pos)
{
	return this->ditance > pos.ditance;
}

bool NGui::StickyNotePositon::operator<(const StickyNotePositon& pos)
{
	return this->ditance < pos.ditance;
}

NGui::StickyNoteRect::StickyNoteRect(QWidget* w)
{
	x = w->x();
	y = w->y();
	width = w->width();
	height = w->height();
}

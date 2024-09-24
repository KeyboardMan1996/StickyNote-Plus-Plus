#include "Action.h"
#include <QIcon>

namespace OriginUI {
	class ActionD {
	public:
		QIcon checkIcon;
		QIcon noCheckIcon;

	};

}

OriginUI::Action::Action()
{
	d = new ActionD();
	connect(this, SIGNAL(toggled(bool)), this, SLOT(actionToggled(bool)));
}

OriginUI::Action::~Action()
{
	delete d;
}

void OriginUI::Action::setCheckIcon(const QIcon& icon)
{
	d->checkIcon = icon;
}

QIcon OriginUI::Action::getCheckIcon()
{
	return d->checkIcon;
}

void OriginUI::Action::setNoCheckIcon(const QIcon& icon)
{
	d->noCheckIcon = icon;
	setIcon(icon);
}

QIcon OriginUI::Action::getNoCheckIcon()
{
	return d->noCheckIcon;
}

void OriginUI::Action::actionToggled(bool checked)
{
	if (checked)
	{
		setIcon(d->checkIcon);
	}else {
		setIcon(d->noCheckIcon);
	}
}


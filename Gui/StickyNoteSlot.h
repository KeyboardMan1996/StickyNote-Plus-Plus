#pragma once
#include <QObject>
namespace NGui {
	class StickyNote;
	class ActionCommand;
	class StickyNoteSlot:public QObject {
		Q_OBJECT
	public:
		StickyNoteSlot();
		~StickyNoteSlot() = default;

	public:
		void connectSlot(ActionCommand* action);
		void setStickyNote(StickyNote* note);
	public Q_SLOTS:
		void actionTrigger(bool check);

	private:
		StickyNote* note;
	};
}
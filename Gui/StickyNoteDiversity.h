#pragma once
#include "StickyNote.h"
#include <memory>
namespace NGui {
	class StickyNoteDiversity :public StickyNote {
	public:
		StickyNoteDiversity(std::shared_ptr<NF::StickyNote> note, QWidget* parent = 0);
		~StickyNoteDiversity();

	protected:
		void initActionCommand() override;
	};
}
#include "StickyNoteDiversity.h"
#include "DiversityContent.h"
NGui::StickyNoteDiversity::StickyNoteDiversity(std::shared_ptr<NF::StickyNote> note, QWidget* parent /*= 0*/)
:StickyNote(note, new DiversityContent(), parent) {

}

NGui::StickyNoteDiversity::~StickyNoteDiversity()
{

}

void NGui::StickyNoteDiversity::initActionCommand()
{

}


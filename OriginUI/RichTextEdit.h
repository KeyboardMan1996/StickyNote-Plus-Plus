#pragma once
#include "SimpleRichTextEdit.h"
namespace OriginUI {
	class RichTextEdit :public SimpleRichTextEdit {
	public:
		RichTextEdit(QWidget* parent = 0);
		~RichTextEdit();
	};
}
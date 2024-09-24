#pragma once
#include "ExportConfig.hpp"
#include <QLineEdit>
#include <QKeyEvent>

namespace OriginUI {
	class KeyboardLineEditD;
	class ORIGIN_UI_EXPORT KeyboardLineEdit :public QLineEdit{
	public:
		KeyboardLineEdit(QWidget* parent = 0);
		~KeyboardLineEdit();
	protected:
		void keyPressEvent(QKeyEvent *event) override;
		void keyReleaseEvent(QKeyEvent* event) override;
	private:
		void changeText();
	private:
		KeyboardLineEditD* d;
	};
}
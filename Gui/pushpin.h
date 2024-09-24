#pragma once
#include <windows.h>
namespace NGui {
	class StickyNote;
	/*
		图钉类，负责将窗口固定到顶层或者最底层
	*/
	class Pushpin {

	public:
		Pushpin();
		~Pushpin();

	public:
		static bool toTop(StickyNote* widget);
		static bool toDesktop(StickyNote* widget);
		static bool toNormal(StickyNote* widget);
		static bool isTop(StickyNote* widget);
		static bool isDesktop(StickyNote* widget);
		static bool isNormal(StickyNote* widget);
	};
}
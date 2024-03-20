#pragma once
#ifndef CONSOLE_PALETTE_H
#define CONSOLE_PALETTE_H

#include <Windows.h>

namespace Color {
	enum {
		BLACK     , DARK_BLUE , GREEN     , BLUE_GREEN,
		BLOOD     , PURPLE    , GOLD      , ORIGINAL  ,
		GRAY      , BLUE      , HIGH_GREEN, SKY_BLUE  ,
		RED       , PLUM      , YELLOW    , WHITE
	};
};

class ConsolePalette {
private:
	unsigned char palette[128] = {};
public:
	ConsolePalette() { memset(palette, Color::BLACK * 0x10 + Color::ORIGINAL, 128); }
	void setPalette(char index, char backColor, char frontColor) { palette[index] = backColor * 0x10 + frontColor; }
	void setColor(char index) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), palette[index]); }
};

#endif // !RGB_COLOR_H
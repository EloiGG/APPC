#pragma once
#include "Log.h"

class GPIO
{
public:
	enum class PinType { Output, Input };
	enum class PinNumber { GPIO_0, GPIO_1, GPIO_2, GPIO_3, GPIO_4, GPIO_5, GPIO_6, GPIO_7 };
	enum class Level { Low, High };
public:
	GPIO();
	~GPIO();
	bool OK;
	bool setPinType(PinNumber number, PinType type);
	bool setPinLevel(PinNumber number, Level level);
	Level getPinLevel(PinNumber number, bool& status);
};


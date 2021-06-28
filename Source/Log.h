/*
  ==============================================================================

	Log.h
	Created: 28 Jun 2021 8:56:29am
	Author:  admin

  ==============================================================================
*/

#pragma once

#include "BottomPanel.h"
#include <JuceHeader.h>

class Log
{
public:
	static void write(const String& string, int debugLevel = 1);
	void writeImpl(const String& string, int debugLevel = 1);
	
	static void update();
	static void title(const String& string);
	static void ln(int debugLevel = 1, int numberOfLines = 1);
	void updateImpl();
	static Log& get() { return _log; }
	String getString(int index) { return s[index-1]; }
	std::function<void()> updateFunction;


private:
	const static int textWidth = 60;
	Log();
	static Log _log;
	String s[2];
};
/*
  ==============================================================================

	Log.h
	Created: 28 Jun 2021 8:56:29am
	Author:  admin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class Log
{
public:
	static constexpr int DEBUG_LEVELS = 2;
	Log(const Log&) = delete;
	static void write(const String& string, int debugLevel = 1);
	void writeImpl(const String& string, int debugLevel);
	
	static void update();
	static void title(const String& string);
	static void ln(int debugLevel = 1, int numberOfLines = 1);
	void updateImpl();
	static Log& get() { return _log; }
	String getString(int index) { return s[index]; }
	std::function<void()> updateFunction;


private:
	const static int textWidth = 60;
	Log();
	static Log _log;
	String s[DEBUG_LEVELS];
};
/*
  ==============================================================================

	Log.cpp
	Created: 28 Jun 2021 8:56:29am
	Author:  admin

  ==============================================================================
*/

#include "Log.h"

Log Log::_log;

void Log::write(const String& string, int debugLevel)
{
	_log.writeImpl(string, debugLevel);
}

void Log::writeImpl(const String& string, int debugLevel)
{
	for (int i = 0; i < debugLevel; i++)
		s[i].append(string, string.length());
}

void Log::update()
{
	_log.updateImpl();
}

void Log::title(const String& string)
{
	auto t = "==================================================";

	String str;
	str.append("\n", 1);
	str.append(t, (textWidth - string.length()) / 2);
	str.append(string.toUpperCase(), string.length());
	str.append(t, (textWidth - string.length()) / 2);
	str.append("\n", 1);
	_log.writeImpl(str);
}

void Log::ln(int debugLevel, int numberOfLines)
{
	for (int i = 0; i < numberOfLines; i++)
		write("\n");
}

void Log::updateImpl()
{
	if (updateFunction)
		updateFunction();
}

Log::Log()
{
	for (int i = 0; i < 2; i++)
		s[i].preallocateBytes(16000);
	title("Debut du programme");
	ln();
	write(Time::getCurrentTime().toString(true, true, true, true));
	ln(1, 3);
}

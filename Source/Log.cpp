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
	_log.writeImpl(string, debugLevel - 1);
}

void Log::writeImpl(const String& string, int debugLevel)
{
	for (int i = debugLevel ; i < DEBUG_LEVELS ; i++)
		s[i].append(string, string.length());
	//logFile.appendText(string);
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
	_log.writeImpl(str, 0);
}

void Log::ln(int debugLevel, int numberOfLines)
{
	for (int i = 0; i < numberOfLines; i++)
		write("\n", debugLevel);
}

void Log::updateImpl()
{
	if (updateFunction)
		updateFunction();
}

Log::Log()
{
	File logDir(File::getCurrentWorkingDirectory().getChildFile("Logs"));
	logDir.createDirectory();
	const String& filename = "Log_APPC_" + Time::getCurrentTime().toString(true, true, true, true).replaceCharacters(" :", "_-") + ".txt";
	logFile = logDir.getChildFile(filename);
	for (int i = 0; i < DEBUG_LEVELS; i++)
		s[i].preallocateBytes(16000);
	title("Debut du programme");
	ln();
	write(Time::getCurrentTime().toString(true, true, true, true));
	ln(1, 3);
}

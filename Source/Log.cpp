/*
  ==============================================================================

	Log.cpp
	Created: 28 Jun 2021 8:56:29am
	Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#include "Log.h"

Log Log::_log;

void Log::writeLn(const String& string)
{
	int elapsedTime(_log._getTime_ms());
	String milliseconds(elapsedTime % 1000);
	String seconds(elapsedTime / 1000 % 60);
	String minutes(elapsedTime / (60 * 1000) % 60);
	_log._writeImpl("\n" + String("[") + minutes + ":" + seconds + ":" + milliseconds + "]\t" + string.removeCharacters("\n"));
}

void Log::writeNext(const String& string)
{
	_log._writeImpl(string);
}

void Log::_writeImpl(const String& string)
{
	if (!init) {
		init = true;
		logFile.create();
		logFile.appendText("[mn:s:ms]");
		title("Debut du programme");
		ln();
		writeLn(Time::getCurrentTime().toString(true, true, true, true));
		ln();
	}
	logFile.appendText(string);
}

void Log::update()
{
	_log._updateImpl();
}

void Log::title(const String& string)
{
	auto t = "==================================================";

	String str;
	str.append("\n\n", 2);
	str.append(t, (textWidth - string.length()) / 2);
	str.append(string.toUpperCase(), string.length());
	str.append(t, (textWidth - string.length()) / 2);
	str.append("\n", 1);
	_log._writeImpl(str);
}

void Log::ln()
{
	writeLn("");
}

void Log::_updateImpl()
{
}

Log::Log() : init(false)
{
	File logDir(File::getCurrentWorkingDirectory().getChildFile("Logs"));
	logDir.createDirectory();
	String time(Time::getCurrentTime().toString(true, true, true, true));
	time = time.replaceFirstOccurrenceOf(":", "h");
	time = time.replaceFirstOccurrenceOf(":", "'");
	time.append("''", 2);
	const String& filename = "Log_APPC_" + time.replaceCharacter(' ', '_') + ".txt";
	logFile = logDir.getChildFile(filename);
}

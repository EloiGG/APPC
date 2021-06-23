/*
  ==============================================================================

	Serial.cpp
	Created: 23 Jun 2021 10:29:11am
	Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#include "SerialThread.h"

SerialThread::SerialThread() : Thread("SerialThread")
{
	setPriority(9);
}

void SerialThread::run()
{
	while (1) {
		DBG("THREAD RUNNING");
		wait(1000);
		if (threadShouldExit())
			break;
	}
}

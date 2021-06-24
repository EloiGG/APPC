/*
  ==============================================================================

	SerialThread.h
	Created: 23 Jun 2021 10:29:11am
	Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Sequence.h"
#include "UART.h"

class SerialThread : public Thread
{
public:
	SerialThread();
	SerialThread(const Sequence& sequence);
	void setSequence(const Sequence& sequence);
	float getProgression() const;
	void askToExit() { exitAsked = true; }
private:
	bool exitAsked;
	float progression;
	virtual void run() override;
	UART uart;
	Sequence sequence;
};
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
#include "Core.h"

class ProgressionThread : public Thread
{
public:
	ProgressionThread() : Thread("ProgressionThread"), progression(0.0f), exitAsked(false) {}
	float getProgression() const { return progression; }
	void askToExit() { exitAsked = true; }
protected:
	void setProgression(float newProgression) { progression = newProgression; }
	bool exitAsked;
private:
	float progression;
};

class SerialThread : public ProgressionThread
{
public:
	SerialThread();
	SerialThread(const Sequence& sequence);
	void setSequence(const Sequence& sequence);
	ErrModule getModuleResponse(int digitNumber, int timeout_ms);
	int getTimeout_ms(unsigned char ordre);
	void setWaitForResponse(bool shouldWaitForResponse) { waitForResponse = shouldWaitForResponse; }
private:
	int timeout_ms;
	bool waitForResponse;
	virtual void run() override;
	Stopwatch s;
	UART uart;
	Sequence sequence;
};
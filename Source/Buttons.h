/*
  ==============================================================================

	Buttons.h
	Created: 21 Jun 2021 12:37:46pm
	Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#pragma once

#include "LookAndFeel.h"
#include "GridTool.h"
#include "Core.h"
#include "SerialThread.h"
#include "Sequence.h"
#include "Log.h"

//==============================================================================
/*
*/
class Progression : public Component, private Timer
{
public:
	Progression(const ProgressionThread& t) : thread(t) { setInterceptsMouseClicks(false, false); }
	void start();
	void paint(juce::Graphics&) override;

private:
	virtual void timerCallback() override;
	const ProgressionThread& thread;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Progression)
};

class Buttons : public juce::Component
{
public:
	Buttons();
	~Buttons() override;

	void paint(juce::Graphics&) override;
	void resized() override;
	void updateVizualisation();
private:
	AlertWindow COMErrorWindow;
	SerialThread sendThread;
	TextButton send, stop;
	TextButton verif;
	GridTool grid;
	Progression progression;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Buttons)
};
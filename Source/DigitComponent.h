/*
  ==============================================================================

	DigitComponent.h
	Created: 17 Jun 2021 11:04:03am
	Author:  admin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "LookAndFeel.h"
#include "Core.h"

//==============================================================================
/*
*/
class DigitEditor : public Label, private Timer
{
public:
	DigitEditor();

	void paint(juce::Graphics&) override;
	void setDigit(const String& newDigit);
	String getDigit();
	void resized() override;


private:
	virtual void timerCallback() override;

	unsigned int frameCounter;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DigitEditor)

};

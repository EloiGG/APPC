/*
  ==============================================================================

	GeneralTab.h
	Created: 19 Jun 2021 10:42:19am
	Author:  Eloi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "GridTool.h"
#include "LookAndFeel.h"
#include "InputComponent.h"
#include "Disabled.h"
#include "Buttons.h"
#include "COMThread.h"
//==============================================================================
/*
*/
class GeneralTab : public juce::Component
{
public:
	GeneralTab();
	~GeneralTab() override;

	void paint(juce::Graphics&) override;
	void resized() override;

	void setNumPrices(int newNumPrices);
	void setNumDigits(int newNumDigits);
	void setCOMPort(int newCOMPort) { COM.setInput(String(newCOMPort)); }

	void updateAllParameters();

private:
	GridTool grid;
	InputComponent nPrices, nDigits, COM;
	CheckBox resetLine;
	Disabled disabled;
	TextButton test, stop;
	Progression testProgression;
	SerialThread testThread;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GeneralTab)
};

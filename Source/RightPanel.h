/*
  ==============================================================================

    RightPanel.h
    Created: 21 Jun 2021 12:36:05pm
    Author:  admin

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

class RightPanel : public juce::Component
{
public:
	RightPanel();
	~RightPanel() override;

	void paint(juce::Graphics&) override;
	void resized() override;

	void setNumPrices(int newNumPrices);
	void setNumDigits(int newNumDigits);
	void setCOMPort(int newCOMPort) { COM.setInput(String(newCOMPort)); }

	void updateParameters();

private:
	Label title;
	GridTool grid;
	InputComponent nPrices, nDigits, COM;
	CheckBox resetLine;
	Disabled disabled;
	TextButton centoFuelOpen;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RightPanel)
};

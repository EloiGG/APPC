/*
  ==============================================================================

    RightPanel.h
    Created: 21 Jun 2021 12:36:05pm
    Author:  Eloi GUIHARD-GOUJON

	Contient la classe pour le panneau de réglages à gauche

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "GridTool.h"
#include "LookAndFeel.h"
#include "InputComponent.h"
#include "Disabled.h"
#include "Buttons.h"

//==============================================================================
/*
*/

/// <summary>
/// Component du panneau de réglages à gauche.
/// </summary>
class LeftPanel : public juce::Component
{
public:
	LeftPanel();
	~LeftPanel() override;

	void paint(juce::Graphics&) override;
	void resized() override;

	// Change le nombre de prix
	void setNumPrices(int newNumPrices);

	// Change le nombre de chiffres par prix
	void setNumDigits(int newNumDigits);

	// Change le port COM
	void setCOMPort(int newCOMPort) { COM.setInput(String(newCOMPort)); }

	void updateParameters();

private:
	Label title;
	GridTool grid;
	InputComponent nPrices, nDigits, COM;
	CheckBox resetLine;
	Disabled disabled;
	TextButton centoFuelOpen;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LeftPanel)
};

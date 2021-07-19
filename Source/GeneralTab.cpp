/*
  ==============================================================================

	GeneralTab.cpp
	Created: 19 Jun 2021 10:42:19am
	Author:  Eloi

  ==============================================================================
*/

#include <JuceHeader.h>
#include "GeneralTab.h"

//==============================================================================
GeneralTab::GeneralTab() : grid(4, 12), nPrices("Nombre de prix", Core::get().getNumPrices()), nDigits("Nombre de chiffres", Core::get().getNumDigits()),
lineControl(CharPointer_UTF8("Contrôle des segments")), resetLine("Effacer si erreur"), COM("Port COM", 3)
{
	addAndMakeVisible(nPrices);
	addAndMakeVisible(nDigits);
	addChildComponent(grid);
	addAndMakeVisible(lineControl);
	addAndMakeVisible(resetLine);
	addAndMakeVisible(disabled);
	disabled.setDisabled(true);

	grid.setBounds(0, 0, getWidth(), 300);

	nPrices.onUpdate = [](const String& input)
	{
		Core::get().setNumPrices(input.getIntValue());
		Core::get().updateVisualization();
	};
	nPrices.min = 1;
	nPrices.max = Core::MAX_PRICES;

	nDigits.onUpdate = [](const String& input)
	{
		Core::get().setNumDigits(input.getIntValue());
		Core::get().updateVisualization();
	};
	nDigits.min = 1;
	nDigits.max = Core::MAX_DIGITS;

	lineControl.onClick = [this]() {Core::get().setLineControl(lineControl.getToggleState()); };
	resetLine.onClick = [this]() {Core::get().setResetLine(resetLine.getState()); };
}

GeneralTab::~GeneralTab()
{
}

void GeneralTab::paint(juce::Graphics& g)
{
	g.fillAll(lfColours::tabBackground);
}

void GeneralTab::resized()
{
	grid.setBounds(grid.getLocalBounds().withWidth(getWidth()));
	nPrices.setBounds(grid.getRectangle(0, 1, 4, 2));
	nDigits.setBounds(grid.getRectangle(0, 2, 4, 3));
	lineControl.setBounds(grid.getRectangle(0, 3, 4, 4));
	resetLine.setBounds(grid.getRectangle(0, 4, 4, 5));
	disabled.setBounds(getLocalBounds());
}

void GeneralTab::setNumPrices(int newNumPrices)
{
	nPrices.setInput(String(newNumPrices));
}

void GeneralTab::setNumDigits(int newNumDigits)
{
	nDigits.setInput(String(newNumDigits));
}

void GeneralTab::updateAllParameters()
{
	auto& c = Core::get();
	setNumPrices(c.getNumPrices());
	setNumDigits(c.getNumDigits());
	resetLine.setToggleState(c.getResetLine(), NotificationType::sendNotification);
	lineControl.setToggleState(c.getLineControl(), NotificationType::sendNotification);
	if (c.getIsInTransmission())
		disabled.setDisabled(true);
	else
		disabled.setDisabled(false);
	repaint();
}

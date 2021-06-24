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
lineControl("Controle des segments"), resetLine("Effacer si erreur"), delay("Delai d'affichage (ms)", 50), id("id", 1)
{
	addAndMakeVisible(nPrices);
	addAndMakeVisible(nDigits);
	addAndMakeVisible(delay);
	addChildComponent(grid);
	addAndMakeVisible(lineControl);
	addAndMakeVisible(resetLine);
	addAndMakeVisible(id);
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

	delay.onUpdate = [](const String& input)
	{
		Core::get().setDelay_ms(input.getIntValue());
	};
	delay.min = 0;
	delay.max = 20000;
	delay.increment = 50;
	delay.decrement = 50;

	id.onUpdate = [](const String& input)
	{
		Core::get().setID(input.getIntValue());
	};
	id.min = 0;
	id.max = 1000;

	lineControl.onStateChange = [this]() {Core::get().setLineControl(lineControl.getState()); };
	resetLine.onStateChange = [this]() {Core::get().setResetLine(resetLine.getState()); };
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
	id.setBounds(grid.getRectangle(0, 0, 4, 1));
	nPrices.setBounds(grid.getRectangle(0, 1, 4, 2));
	nDigits.setBounds(grid.getRectangle(0, 2, 4, 3));
	lineControl.setBounds(grid.getRectangle(0, 3, 4, 4));
	resetLine.setBounds(grid.getRectangle(0, 4, 4, 5));
	delay.setBounds(grid.getRectangle(0, 5, 4, 6));
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
	if (c.getIsInTransmission())
		disabled.setDisabled(true);
	else
		disabled.setDisabled(false);
	repaint();
}

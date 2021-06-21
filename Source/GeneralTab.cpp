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
GeneralTab::GeneralTab() : grid(4, 12), nPrices("Nombre de prix", Core::get().getNumPrices()), nDigits("Nombre de chiffres", Core::get().getNumDigits())
{
	addAndMakeVisible(nPrices);
	addAndMakeVisible(nDigits);
	grid.setBounds(0, 0, getWidth(), 300);

	nPrices.onUpdate = [](String& input)
	{
		Core::get().setNumPrices(input.getIntValue());
		Core::get().updateVisualization();
	};
	nPrices.min = 1;
	nPrices.max = Core::MAX_PRICES;

	nDigits.onUpdate = [](String& input)
	{
		Core::get().setNumDigits(input.getIntValue());
		Core::get().updateVisualization();
	};
	nDigits.min = 2;
	nDigits.max = Core::MAX_DIGITS;

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
	nPrices.setBounds(grid.getRectangle(0, 0, 4, 1));
	nDigits.setBounds(grid.getRectangle(0, 1, 4, 2));
}

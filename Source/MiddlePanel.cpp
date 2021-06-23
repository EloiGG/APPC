/*
  ==============================================================================

	MiddlePanel.cpp
	Created: 19 Jun 2021 1:07:43am
	Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#include "MiddlePanel.h"

//==============================================================================
MiddlePanel::MiddlePanel()
{
	addAndMakeVisible(prices);
}

MiddlePanel::~MiddlePanel()
{
}

void MiddlePanel::paint(juce::Graphics& g)
{
	g.fillAll(lfColours::panelBackground);
}

void MiddlePanel::resized()
{
	prices.setBounds(prices.getFittingRectangle(getLocalBounds()));
}

void MiddlePanel::updatePrices(TextUpdateOrigin whocalled, unsigned int index)
{
	prices.updatePrices(whocalled, index);
}

void MiddlePanel::updateVisualization()
{
	prices.setNumPrices(Core::get().getNumPrices());
	prices.setNumDigits(Core::get().getNumDigits());
}

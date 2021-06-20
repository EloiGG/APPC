/*
  ==============================================================================

    MiddlePanel.cpp
    Created: 19 Jun 2021 1:07:43am
    Author:  Eloi

  ==============================================================================
*/

#include "MiddlePanel.h"

//==============================================================================
MiddlePanel::MiddlePanel()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    prices.setNumPrices(5);
    prices.setNumDigits(6);
    addAndMakeVisible(prices);

}

MiddlePanel::~MiddlePanel()
{
}

void MiddlePanel::paint (juce::Graphics& g)
{
    g.fillAll(lfColours::panelBackground);
}

void MiddlePanel::resized()
{
    prices.setBounds(prices.getFittingRectangle(getLocalBounds()));
}

void MiddlePanel::updatePrices(TextUpdateOrigin origin)
{
    prices.updatePrices(origin);
}

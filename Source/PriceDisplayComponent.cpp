/*
  ==============================================================================

	PriceDisplayComponent.cpp
	Created: 17 Jun 2021 9:16:08am
	Author:  admin

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PriceDisplayComponent.h"

//==============================================================================
PriceDisplayComponent::PriceDisplayComponent(unsigned int num_of_prices) : grid(1, num_of_prices), numPrices(num_of_prices)
{
	addAndMakeVisible(grid);
	for (int i = 0; i < numPrices; i++) {
		addAndMakeVisible(prices[i]);
	}
}

PriceDisplayComponent::~PriceDisplayComponent()
{
}

void PriceDisplayComponent::paint(juce::Graphics& g)
{
}

void PriceDisplayComponent::resized()
{
	grid.setBounds(getLocalBounds());

	auto margins = getHeight() * 0.01;
	for (int i = 0; i < numPrices; i++)
		prices[i].setBounds(grid.getRectangle(0, i, 1, i + 1).reduced(margins)); // setbounds
}

void PriceDisplayComponent::setNumPrices(unsigned int num_of_prices)
{
	if (num_of_prices <= MAX_PRICES) {
		grid.resize(num_of_prices, grid.getNumRows());
		numPrices = num_of_prices;
	}
}

void PriceDisplayComponent::addPrice()
{
	setNumPrices(++numPrices);
}

void PriceDisplayComponent::setNumDigits(unsigned int num_of_digits)
{
}

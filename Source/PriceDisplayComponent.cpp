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
PriceDisplayComponent::PriceDisplayComponent(unsigned int num_of_prices) : grid(1, num_of_prices), numPrices(num_of_prices), prices()
{
	init();
}

PriceDisplayComponent::~PriceDisplayComponent()
{
}

void PriceDisplayComponent::init()
{
	//addAndMakeVisible(grid);
	addChildComponent(grid);
	for (int i = 0; i < numPrices; i++) {
		addAndMakeVisible(prices[i]);
		prices[i].setTabOrder(i + 1);
		prices[i].setID(i);
		prices[i].setPrice(String("2231"));
	}
}

void PriceDisplayComponent::paint(juce::Graphics& g)
{
	g.fillAll(lfColours::priceDisplayBackground);
	g.setColour(Colours::black);
	for (int i = 0; i < numPrices; i++)
		g.drawLine(Line<float>(grid.getPoint(0,i).toFloat(), grid.getPoint(1,i).toFloat()), 1.0f);
	g.drawRect(getLocalBounds());
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
	if (num_of_prices <= MAX_PRICES && num_of_prices != numPrices) {
		grid.resize(grid.getNumColumns(), num_of_prices);
		numPrices = num_of_prices;
		Core::get().setNumPrices(numPrices);
		init();
	}
}

void PriceDisplayComponent::addPrice()
{
	setNumPrices(++numPrices);
}

void PriceDisplayComponent::setNumDigits(unsigned int num_of_digits)
{

	for (int i = 0; i < numPrices; i++)
		prices[i].setNumberOfDigits(num_of_digits);
}

void PriceDisplayComponent::hideAllDigits(bool shouldHideDigits)
{
	for (int i = 0; i < numPrices; i++)
		prices[i].hideDigits(true);
}

Rectangle<int> PriceDisplayComponent::getFittingRectangle(const Rectangle<int>& rect)
{
	float spacingBetwinNumbers = 1.1;
	float width = numPrices, height = prices[0].getNumDigits()* spacingBetwinNumbers, ratio = width/height;
	Rectangle<int> r(10, 10, width, height);

	r.setCentre(rect.getCentre());
	int exp = std::min((rect.getWidth() - width), (rect.getHeight() - height)/ratio)/2;
	r.expand(exp,exp*ratio);
	
	return r;
}

void PriceDisplayComponent::updatePrices(TextUpdateOrigin o)
{
	if (o == TextUpdateOrigin::PriceEditor)
		DBG("call");
}

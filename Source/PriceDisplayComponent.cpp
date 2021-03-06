/*
  ==============================================================================

	PriceDisplayComponent.cpp
	Created: 17 Jun 2021 9:16:08am
	Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PriceDisplayComponent.h"

//==============================================================================
PriceDisplayComponent::PriceDisplayComponent() : numPrices(0), grid(1, 0), prices{ 0,1,2,3,4,5,6,7,8,9 }
{
	Core::get().setModuleState = [this](int moduleNumber, const ErrModule& newState)
	{
		prices[moduleNumber / Core::get().getNumDigits()].setModuleState(moduleNumber % Core::get().getNumDigits(), newState);
	};
	Core::get().getModuleState = [this](int moduleNumber)
	{
		auto r = prices[moduleNumber / Core::get().getNumDigits()].getModuleState(moduleNumber % Core::get().getNumDigits());
		return r;
	};
	addChildComponent(grid);
	init();
}

PriceDisplayComponent::~PriceDisplayComponent()
{
}

void PriceDisplayComponent::init()
{
	for (int i = 0; i < numPrices; i++) {
		prices[i].setID(i);
		prices[i].setNumberOfDigits(Core::get().getNumDigits());
		addAndMakeVisible(prices[i]);
	}
	for (int i = numPrices; i < Core::MAX_PRICES; i++) {
		prices[i].setID(i);
		prices[i].setVisible(false);
	}
	if (getParentComponent())
		getParentComponent()->resized();
}

void PriceDisplayComponent::paint(juce::Graphics& g)
{
	g.fillAll(lfColours::priceDisplayBackground);
	g.setColour(Colours::black);
	for (int i = 0; i < numPrices; i++)
		g.drawLine(Line<float>(grid.getPoint(0, i).toFloat(), grid.getPoint(1, i).toFloat()), 1.0f);
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


juce::Rectangle<int> PriceDisplayComponent::getFittingRectangle(const juce::Rectangle<int>& rect)
{
	float spacingBetwinNumbers = 1.1;
	float width = numPrices, height = (Core::get().getNumDigits())  * spacingBetwinNumbers, ratio = width / height;
	juce::Rectangle<int> r(10, 10, width, height);

	r.setCentre(rect.getCentre());
	int exp = jmin((rect.getWidth() - width), (rect.getHeight() - height) / ratio) / 2;
	r.expand(exp, exp * ratio);

	return r;
}

void PriceDisplayComponent::updatePrices(TextUpdateOrigin whoCalled, unsigned int priceIndex)
{
	for (int i = 0; i < Core::get().getNumPrices(); i++) {
		prices[i].updatePrices(whoCalled, priceIndex);
	}
}

void PriceDisplayComponent::setModuleState(int moduleNumber, const ErrModule& newState)
{
	prices[moduleNumber / Core::get().getNumDigits()].setModuleState(moduleNumber / Core::get().getNumDigits(), newState);
}

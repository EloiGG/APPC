/*
  ==============================================================================

	Core.cpp
	Created: 19 Jun 2021 11:11:52am
	Author:  Eloi

  ==============================================================================
*/

#include "Core.h"

Core& Core::get()
{
	static Core core_instance;
	return core_instance;
}

unsigned int Core::getNumDigits()
{
	return numDigits;
}

void Core::setNumDigits(unsigned int newNumDigits)
{
	if (newNumDigits <= MAX_DIGITS)
		numDigits = newNumDigits;
}

unsigned int Core::getNumPrices()
{
	return numPrices;
}

void Core::setNumPrices(unsigned int newNumPrices)
{
	if (newNumPrices <= MAX_PRICES)
		numPrices = newNumPrices;
}

Price Core::getPrice(unsigned int index)
{
	if (index < numDigits)
		return prices[index];
	return Price("0");
}

void Core::updatePrices(TextUpdateOrigin whoCalled)
{
	pricesUpdateFunction(whoCalled);
}

void Core::setUpdatePriceFunction(const std::function<void(TextUpdateOrigin)>& f)
{
	pricesUpdateFunction = f;
}

Core::Core() : numDigits(4), numPrices(4)
{
}

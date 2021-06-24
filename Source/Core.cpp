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
	if (index < numPrices)
		return prices[index];
	return Price("0");
}

void Core::setPrice(unsigned int index, const Price& newPrice)
{
	prices[index] = newPrice;
}

unsigned int Core::getDelay()
{
	return delay_ms;
}

void Core::setDelay_ms(unsigned int newDelay)
{
	delay_ms = newDelay;
}

bool Core::getIsInTransmission()
{
	return isInTransmission;
}

void Core::setInTransmission(bool shouldBeInTransmission)
{
	isInTransmission = shouldBeInTransmission;
}

void Core::updatePrices(TextUpdateOrigin whoCalled, unsigned int priceIndex)
{
	if (pricesUpdateFunction)
		pricesUpdateFunction(whoCalled, priceIndex);
}

void Core::setUpdatePriceFunction(const std::function<void(TextUpdateOrigin, unsigned int)>& f)
{
	pricesUpdateFunction = f;
}

Network Core::getNetwork()
{
	return network;
}

void Core::setNetwork(const Network& net)
{
	network = net;
	networkInit = true;
}

bool Core::hasNetwork()
{
	return networkInit;
}

void Core::loadInformationsFromNetwork()
{
	if (!networkInit) return;

	auto s = network.getFuelPrice();
	PricesJSON p(s);
	numPrices = p.getNumPrices();
	unsigned int nd = 0;
	setNumPrices(p.getNumPrices());
	for (int i = 0; i < numPrices; i++) {
		prices[i] = p[i].getPrice();
		if (p[i].getPrice().length() > nd) nd = p[i].getPrice().length();
	}
	setNumDigits(nd - 1);
	for (int i = 0; i < numPrices; i++)
		updatePrices(TextUpdateOrigin::Omni, i);
	updateVisualization();
}

std::shared_ptr<APPCLookAndFeel> Core::getLookAndFeel()
{
	return lfptr;
}

Core::Core() : numDigits(4), numPrices(4), lfptr(new APPCLookAndFeel),
networkInit(false), delay_ms(0)
{
}

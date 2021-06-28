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

void Core::saveConfigJSON(const File& f)
{
	f.deleteFile();
	f.create();
	f.replaceWithText(configjson->makeConfigJSON(id, configjson->getBaseAPI(), network.getPassword(), lineControl, resetLine, configjson->getDelay()));
}

void Core::loadInformationsFromNetwork()
{
	if (!networkInit) return;
	Log::write(L"Chargement des informations depuis le r�seau...\n\n");
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
	if (updateVisualization)
		updateVisualization();
}

void Core::loadInformationsFromJSON()
{
	Log::write(L"Lecture des informations du fichier JSON. Param�tres d�tect�s : \n", 2);
	auto pwrd = configjson->getAuthPassword();
	auto id_ = configjson->getID();
	auto baseAPI = configjson->getBaseAPI();
	auto reset_line = configjson->getResetLine();
	auto line_control = configjson->getLineControl();
	auto delay = configjson->getDelay();

	if (pwrd != "error") {
		Log::write("Mot de passe\n",2);
		setNetwork(Network("X-AUTH-TOKEN", pwrd));
	}
	if (id_ != -1) {
		Log::write("ID\n", 2);
		id = id_;
	}
	if (baseAPI != "error") {
		Log::write("Adresse API\n", 2);
	// a faire
	}
	if (reset_line != -1) {
		Log::write("Reset line\n", 2);
		resetLine = reset_line;
	}
	if (line_control != -1) {
		Log::write("Line control\n", 2);
		lineControl = line_control;
	}
	if (delay != -1) {
		Log::write("Delay\n", 2);
		delay_ms = delay;
	}

	Log::ln(2, 1);

	for (int i = 0; i < numPrices; i++)
		updatePrices(TextUpdateOrigin::Omni, i);
	if (updateVisualization)
		updateVisualization();
}

void Core::setSequence(const Sequence& newSequence)
{
	sequence = newSequence;
}

std::shared_ptr<APPCLookAndFeel> Core::getLookAndFeel()
{
	return lfptr;
}

Core::Core() : numDigits(4), numPrices(4), lfptr(new APPCLookAndFeel),
networkInit(false), delay_ms(0), configjson(nullptr)
{
}

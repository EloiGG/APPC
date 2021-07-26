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
	setSequenceDelay(newDelay);
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

Network Core::getNetwork() const
{
	return network;
}

void Core::setNetwork(const Network& net)
{
	network = net;
	Log::write(CharPointer_UTF16(CharPointer_UTF16(L"\nV\u00E9rification de la configuration r\u00E9seau...\n")));

	if (std::get<0>(net.connected())) {
		connected = true;
		Log::write(CharPointer_UTF16(L"Connect\u00E9 \u00E0 CentoFuel\n"));
	}
	else {
		Log::write(CharPointer_UTF16(L"Impossible de se connecter \u00E0 CentoFuel\n"));
		String errorMessage;
		switch (std::get<1>(net.connected())) // code erreur	
		{
		case 404:
			errorMessage = "Resource not found";
			break;
		case 403:
			errorMessage = "Access denied";
			break;
		case 401:
			errorMessage = "Authentication problems";
			break;
		case 400:
			errorMessage = "Bad Type";
			break;
		default:
			break;
		}
		Log::write("Message d'erreur : ");
		Log::write(errorMessage);
	}
	Log::update();
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
	f.replaceWithText(configjson->makeConfigJSON(id, configjson->getBaseAPI(), "", lineControl, resetLine, configjson->getDelay(), numDigits, numPrices, COM));
}

void Core::savePriceSave(const File& f)
{
	String p[MAX_PRICES];
	for (int i = 0; i < MAX_PRICES; i++)
		p[i] = prices[i].toString(MAX_DIGITS);
	f.deleteFile();
	f.create();
	f.replaceWithText(PriceSave::makePriceSave(p, numPrices));
}

void Core::loadInformationsFromNetwork()
{
	if (!networkInit || !connected) return;

	if (gsjson)
		delete gsjson;
	gsjson = new GasStationsJSON(network.getAllGasStations());
	gsjson->updatePropreties(network);

	//PanelJSON p(s);
	//panelsjson = new PanelsJSON(network.getPanel(0));

	/*Log::write(CharPointer_UTF16(L"Chargement des informations depuis le r\u00E9seau...\n\n"));
	auto s = network.getFuelPrice();
	PricesJSON p(s);
	if (pricesjson != nullptr)
		delete pricesjson;
	pricesjson = new PricesJSON(p);
	numPrices = p.getNumPrices();
	unsigned int nd = 0;
	setNumPrices(p.getNumPrices());
	for (int i = 0; i < numPrices; i++) {
		prices[i] = p[i].getPrice();
		if (p[i].getPrice().length() > nd) nd = p[i].getPrice().length();
	}
	setNumDigits(nd - 1);
	for (int i = 0; i < numPrices; i++)
		updatePrices(TextUpdateOrigin::Omni, i);*/
	if (updateVisualization)
		updateVisualization();
}

void Core::loadInformationsFromJSON()
{
	Log::write(CharPointer_UTF16(L"Lecture des informations du fichier JSON. Param\u00E8tres d\u00E9tect\u00E9s : \n"), 2);
	auto pwrd = configjson->getAuthPassword();
	auto id_ = configjson->getID();
	auto baseAPI = configjson->getBaseAPI();
	auto reset_line = configjson->getResetLine();
	auto line_control = configjson->getLineControl();
	auto delay = configjson->getDelay();
	auto numLines = configjson->getNumLines();
	auto numColumns = configjson->getNumColumns();
	auto portCOM = configjson->getCOMPort();
	bool hasNetwork = false;
	if (pwrd != "error") {
		Log::write("Mot de passe\n", 2);
		hasNetwork = true;
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
		Log::write("Delai\n", 2);
		delay_ms = delay;
	}
	if (numLines != -1) {
		Log::write("Nombre de lignes\n", 2);
		numPrices = numLines;
	}
	if (numColumns != -1) {
		Log::write("Nombre de colonnes\n", 2);
		numDigits = numColumns;
	}
	if (COM != -1) {
		Log::write("Port COM\n", 2);
		COM = portCOM;
	}

	Log::ln(2, 1);

	if (hasNetwork)
		setNetwork(Network("X-AUTH-TOKEN", pwrd));

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

bool Core::getBatteryAlarm()
{
	bool b = false;
	if (b == false)
		Log::write("erreur lecture pin");
	return false;
}

Core::Core() : numDigits(4), numPrices(4), lfptr(new APPCLookAndFeel),
networkInit(false), delay_ms(0), configjson(nullptr), pricesjson(nullptr), connected(false), inTesting(false), initBool(false),
inSelection(false)
{
	digitEditorAcceptedCharacters = " abcdefghijklnopqrstuvwxyzABCDEFGHIJKLNOPQRSTUVWXYZ0123456789-.,";
}

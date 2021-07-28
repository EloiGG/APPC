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
	if (updateVisualization)
		updateVisualization();
}

void Core::loadInformationsFromJSON()
{
	Log::write(CharPointer_UTF16(L"Lecture des informations du fichier JSON. Param\u00E8tres d\u00E9tect\u00E9s : \n"), 2);
	auto pwrd = configjson->getAuthPassword();
	auto baseAPI = configjson->getBaseAPI();
	auto line_control = configjson->getLineControl();
	auto numLines = configjson->getNumLines();
	auto numColumns = configjson->getNumColumns();
	auto portCOM = configjson->getCOMPort();
	auto startingCharacter = configjson->getStartingCharacter();

	bool hasNetwork = false;

	if (pwrd != "error" && pwrd.length()) {
		Log::write("Mot de passe\n", 2);
		hasNetwork = true;
	}

	if (baseAPI != "error") {
		Log::write("Adresse API\n", 2);
	}
	else 
		hasNetwork = false;
	
	if (line_control != -1) {
		Log::write("Line control\n", 2);
		lineControl = line_control;
	}
	else
		lineControl = true;

	if (numLines != -1) {
		Log::write("Nombre de lignes\n", 2);
		if (numLines > Core::MAX_PRICES) numPrices = Core::MAX_PRICES;
		else if (numLines <= 0) numPrices = 1;
		else numPrices = numLines;
	}
	else
		numPrices = 4;

	if (numColumns != -1) {
		Log::write("Nombre de colonnes\n", 2);
		if (numColumns > Core::MAX_DIGITS) numPrices = Core::MAX_DIGITS;
		else if (numColumns <= 0) numPrices = 1;
		else numDigits = numColumns;
	}
	else
		numDigits = 4;

	if (portCOM != -1) {
		Log::write("Port COM\n", 2);
		if (portCOM >= 256) COM = 255;
		else if (portCOM <= 0) COM = 1;
		else COM = portCOM;
	}
	else
		COM = 3;

	if (startingCharacter != "error") {
		Log::write("Caractere a afficher au demarrage\n", 2);
		Price p(startingCharacter);
		char s[Core::MAX_DIGITS + 1];
		char c(Price::toUARTchar(startingCharacter.getCharPointer()[0]));
		for (int i = 0; i < Core::MAX_DIGITS; ++i)
			s[i] = c;
		s[Core::MAX_DIGITS] = '\0';
		for (int i = 0; i < Core::MAX_PRICES; ++i)
			prices[i] = String(s);
	}
	else
		for (int i = 0; i < Core::MAX_PRICES; ++i)
			prices[i] = "888888";

	Log::ln(2, 1);

	if (hasNetwork)
		setNetwork(Network("X-AUTH-TOKEN", pwrd, baseAPI));
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

Sequence Core::createOptimizedSequence()
{
	Sequence s;

	for (int p = 0; p < numPrices; ++p) {
		for (int d = 0; d < numDigits; ++d) {
			int moduleNumber = d + p * numDigits;
			DBG("prix : " << p << " digit : " << d << " adresse : " << moduleNumber);
			auto state = getModuleState(moduleNumber);
			if (!state.upToDate || state.work_in_progress || state.stopping)
				s.addStep(Sequence::SequenceStep(moduleNumber + 0x30, lineControl ? 0x46 : 0x43,
					prices[p].getUARTchar(d)));
		}
	}
	return s;
}

Core::Core() : numDigits(4), numPrices(4), lfptr(new APPCLookAndFeel),
networkInit(false), delay_ms(0), configjson(nullptr), pricesjson(nullptr), connected(false), inTesting(false), initBool(false),
inSelection(false), resetLine(false)
{
	digitEditorAcceptedCharacters = " abcdefghijklnopqrstuvwxyzABCDEFGHIJKLNOPQRSTUVWXYZ0123456789-.";
}

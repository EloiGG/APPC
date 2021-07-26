/*
  ==============================================================================

	Core.h
	Created: 19 Jun 2021 11:11:52am
	Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#pragma once

#include "Price.h"
#include <functional>
#include "LookAndFeel.h"
#include "Networking.h"
#include "JSON.h"
#include "Sequence.h"
#include "SpecialLabel.h"
#include "AlertWindows.h"

struct ErrModule
{
	static ErrModule white() { ErrModule r; r.work_in_progress = true; return r; }
	static ErrModule notUpToDate() { ErrModule r; r.upToDate = false; return r; }
	bool err_ok = 0;
	bool work_in_progress = 0;
	bool stopping = 0;
	bool upToDate = 0;
	bool erreurs[9] = { 0 };
	enum {
		err_A = 0,
		err_B = 1,
		err_C = 2,
		err_D = 3,
		err_E = 4,
		err_F = 5,
		err_G = 6,
		err_reponse = 7,
		err_illisible = 8
	};
};

enum class TextUpdateOrigin
{
	PriceEditor, DigitEditor, InputComponent, Omni
};

class Core
{
public:
	static constexpr size_t MAX_DIGITS = 6;
	static constexpr size_t MAX_PRICES = 10;

	Core(const Core&) = delete;
	static Core& get();
	~Core() {  }
	void kill() { delete configjson; delete pricesjson; delete gsjson; }

	unsigned int getNumDigits();
	void setNumDigits(unsigned int newNumDigits);

	unsigned int getNumPrices();
	void setNumPrices(unsigned int newNumPrices);

	Price getPrice(unsigned int index);
	void setPrice(unsigned int index, const Price& newPrice);

	unsigned int getDelay();
	void setDelay_ms(unsigned int newDelay);

	bool getLineControl() { return lineControl; }
	void setLineControl(bool newLineControl) { lineControl = newLineControl; }

	bool getResetLine() { return resetLine; }
	void setResetLine(bool newResetLine) { resetLine = newResetLine; }

	int getCurrentStationID() { return stationID; }
	void setCurrentStationID(int newStationID) { stationID = newStationID; }

	int getCurrentPanelID() { return panelID; }
	void setCurrentPanelID(int newPanelID) { panelID = newPanelID; }

	bool getIsInTransmission();
	void setInTransmission(bool shouldBeInTransmission);

	String getUCName() { return UCName; }
	void setUCName(const String& newName) { UCName = newName; }

	void setID(int newID) { id = newID; }
	int getID() { return id; }

	void setCOMPort(unsigned int newComPort) { COM = newComPort; }
	unsigned int getCOMPort() { return COM; }

	Network getNetwork() const;
	void setNetwork(const Network& net);

	bool hasNetwork();

	void setConfigJSON(const File& f) { if (configjson != nullptr) delete configjson; configjson = new ConfigJSON(f); }
	void saveConfigJSON(const File& f);

	void savePriceSave(const File& f);

	void loadInformationsFromNetwork();
	void loadInformationsFromJSON();

	void setSequence(const Sequence& newSequence);
	Sequence getSequence() { return sequence; }

	void setSequenceDelay(unsigned int newDelay) { sequence.setDelay(newDelay); }

	std::shared_ptr<APPCLookAndFeel> getLookAndFeel();

	void updatePrices(TextUpdateOrigin whoCalled, unsigned int priceIndex);
	void setUpdatePriceFunction(const std::function<void(TextUpdateOrigin, unsigned int)>& f);
	std::function<void()> updateVisualization;
	std::function<void(int, const ErrModule&)> setModuleState;
	std::function<void()> sendSequence;
	std::function<void()> openSettings;
	std::function<void()> closeSettings;
	std::function<void(SpecialLabel*, const String&, unsigned int)> showKeyboard;
	std::function<void()> selectGasStation;
	std::function<void()> selectPanel;
	std::function<void()> selectUC;
	std::function<void()> closeUCSelection;
	std::function<void()> closeAllSelections;

	void setPlaySequence(bool shouldPlaySequence) { playSequence = shouldPlaySequence; }
	bool getPlaySequence() { return playSequence; }

	Price* getPrices() { return prices; }
	PricesJSON* getpricesjson() { return pricesjson; }
	GasStationsJSON* getGasStationsjson() { return gsjson; }
	PanelJSON getPanelsjson(int index) { return network.getPanels(index); }
	UCsJSON getUCsjson(int index) { return network.getUCs(index); }

	bool isConnected() { return connected; }
	void setConnected(bool shouldBeConnected) { connected = shouldBeConnected; }

	bool isInSelection() { return inSelection; }
	void setInSelection(bool shouldBeInSelection) { inSelection = shouldBeInSelection; }

	bool getBatteryAlarm();
	StringRef getDigitEditorAcceptedCharacters() { return digitEditorAcceptedCharacters; }

	void setInTesting(bool shouldBeInTesting) { inTesting = shouldBeInTesting; }
	bool getInTesting() { return inTesting; }

	void init() { initBool = true; }
	void resetInit() {
		initBool = false;
		for (int i = 0; i < numPrices * numDigits; i++){
				setModuleState(i, ErrModule::white());
				DBG(int(i));
			}
		updateVisualization();
	}
	bool isInit() { return initBool; }


	void openAlertWindow(APPCAlertWindows::WindowType window, const std::function<void(int)>& callbackfunction = std::function<void(int)>())
	{
		alertWindows.open(window, callbackfunction);
	}
private:
	Core();

	unsigned int numDigits, numPrices, delay_ms, id, COM, stationID, panelID;
	bool networkInit, lineControl, resetLine, isInTransmission, playSequence, connected, inTesting, initBool, inSelection;
	std::function<void(TextUpdateOrigin, unsigned int)> pricesUpdateFunction;
	ConfigJSON* configjson;
	PricesJSON* pricesjson;
	GasStationsJSON* gsjson;
	Network network;
	Price prices[MAX_PRICES];
	std::shared_ptr<APPCLookAndFeel> lfptr;
	Sequence sequence;
	String digitEditorAcceptedCharacters, UCName;
	APPCAlertWindows alertWindows;
	JUCE_LEAK_DETECTOR(Core)
};
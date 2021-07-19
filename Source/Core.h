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
#include "GPIO.h"
#include "SpecialLabel.h"

struct ErrModule
{
    bool err_ok = 0;
    bool work_in_progress = 0;
    bool stopping = 0;
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
    void kill() { delete configjson; delete pricesjson; }

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
    
    bool getIsInTransmission();
    void setInTransmission(bool shouldBeInTransmission);
    
    void setID(int newID) { id = newID; }
    int getID() { return id; }
    
    Network getNetwork();
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

    void setPlaySequence(bool shouldPlaySequence) { playSequence = shouldPlaySequence; }
    bool getPlaySequence() { return playSequence; }

    Price* getPrices() { return prices; }

    PricesJSON* getpricesjson(){ return pricesjson; }

    bool isConnected() { return connected; }
    void setConnected(bool shouldBeConnected) { connected = shouldBeConnected; }

    bool getBatteryAlarm();
    StringRef getDigitEditorAcceptedCharacters() { return digitEditorAcceptedCharacters; }

private:
    Core();

    static constexpr GPIO::PinNumber ALARM_PIN = GPIO::PinNumber::GPIO_6;
    static constexpr GPIO::PinNumber DOOR_PIN = GPIO::PinNumber::GPIO_3;

    unsigned int numDigits, numPrices, delay_ms, id;
    bool networkInit, lineControl, resetLine, isInTransmission, playSequence, connected;
    std::function<void(TextUpdateOrigin, unsigned int)> pricesUpdateFunction;
    ConfigJSON* configjson;
    PricesJSON* pricesjson;
    Network network;
    Price prices[MAX_PRICES];
    std::shared_ptr<APPCLookAndFeel> lfptr;
    Sequence sequence;
    GPIO gpio;
    String digitEditorAcceptedCharacters;
    JUCE_LEAK_DETECTOR(Core)
};
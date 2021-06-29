/*
  ==============================================================================

    Core.h
    Created: 19 Jun 2021 11:11:52am
    Author:  Eloi

  ==============================================================================
*/

#pragma once
#ifndef CORE_H
#define CORE_H

#include "Price.h"
#include <functional>
#include "LookAndFeel.h"
#include "Networking.h"
#include "JSON.h"
#include "Sequence.h"

struct ErrModule
{
    bool err_ok = 0;
    bool work_in_progress = 0;
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
    void kill() { delete configjson; }


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
    void updatePrices(TextUpdateOrigin whoCalled, unsigned int priceIndex);
    void setUpdatePriceFunction(const std::function<void(TextUpdateOrigin, unsigned int)>& f);

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

    std::shared_ptr<APPCLookAndFeel> getLookAndFeel();
    std::function<void()> updateVisualization;

    void setPlaySequence(bool shouldPlaySequence) { playSequence = shouldPlaySequence; }
    bool getPlaySequence() { return playSequence; }

    std::function<void(int, const ErrModule&)> setModuleState;

private:
    std::function<void(TextUpdateOrigin, unsigned int)> pricesUpdateFunction;
    Core();
    ConfigJSON* configjson;
    Network network;
    unsigned int numDigits, numPrices, delay_ms, id;
    bool networkInit, lineControl, resetLine, isInTransmission, playSequence;
    Price prices[MAX_PRICES];
    std::shared_ptr<APPCLookAndFeel> lfptr;
    Sequence sequence;
    JUCE_LEAK_DETECTOR(Core)
};



#endif // !CORE_H
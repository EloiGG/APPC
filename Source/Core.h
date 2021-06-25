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

    void setJSON(const File& f) { if (configjson != nullptr) delete configjson; configjson = new ConfigJSON(f); }
    void saveConfigJSON(const File& f);

    void loadInformationsFromNetwork();
    void loadInformationsFromJSON();

    std::shared_ptr<APPCLookAndFeel> getLookAndFeel();
    std::function<void()> updateVisualization;


private:
    std::function<void(TextUpdateOrigin, unsigned int)> pricesUpdateFunction;
    Core();
    ConfigJSON* configjson;
    Network network;
    unsigned int numDigits, numPrices, delay_ms, id;
    bool networkInit, lineControl, resetLine, isInTransmission;
    Price prices[MAX_PRICES];
    std::shared_ptr<APPCLookAndFeel> lfptr;

    JUCE_LEAK_DETECTOR(Core)
};



#endif // !CORE_H
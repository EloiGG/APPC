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

    unsigned int getNumDigits();
    void setNumDigits(unsigned int newNumDigits);

    unsigned int getNumPrices();
    void setNumPrices(unsigned int newNumPrices);

    Price getPrice(unsigned int index);
    void setPrice(unsigned int index, const Price& newPrice);

    void updatePrices(TextUpdateOrigin whoCalled, unsigned int priceIndex);
    void setUpdatePriceFunction(const std::function<void(TextUpdateOrigin, unsigned int)>& f);

    Network getNetwork();
    void setNetwork(const Network& net);
    bool hasNetwork();

    void loadInformationsFromNetwork();

    std::shared_ptr<APPCLookAndFeel> getLookAndFeel();
    std::function<void()> updateVisualization;
private:
    std::function<void(TextUpdateOrigin, unsigned int)> pricesUpdateFunction;
    Core();

    Network network;
    unsigned int numDigits, numPrices;
    bool networkInit;
    Price prices[MAX_PRICES];
    std::shared_ptr<APPCLookAndFeel> lfptr;
};

#endif // !CORE_H
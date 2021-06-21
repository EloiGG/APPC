/*
  ==============================================================================

    PriceDisplayComponent.h
    Created: 17 Jun 2021 9:16:08am
    Author:  admin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "GridTool.h"
#include "PriceComponent.h"

//==============================================================================
/*
*/

class PriceDisplayComponent  : public juce::Component
{
    static constexpr size_t MAX_PRICES = 10;
public:
    PriceDisplayComponent();
    ~PriceDisplayComponent() override;
    void init();

    void paint (juce::Graphics&) override;
    void resized() override;

    void setNumPrices(unsigned int num_of_prices);
    void addPrice();

    void setNumDigits(unsigned int num_of_digits);

    Rectangle<int> getFittingRectangle(const Rectangle<int>& r);

    void updatePrices(TextUpdateOrigin whoCalled, unsigned int priceIndex);


private:
    PriceComponent prices[MAX_PRICES];
    GridTool grid;
    size_t numPrices;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PriceDisplayComponent)
};
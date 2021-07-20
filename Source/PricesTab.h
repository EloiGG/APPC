/*
  ==============================================================================

    PriceTab.h
    Created: 19 Jun 2021 10:42:31am
    Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PriceTab.h"
#include "Core.h"
#include "LookAndFeel.h"
#include "JSON.h"
#include "Disabled.h"
//==============================================================================
/*
*/
class PricesTab  : public juce::TabbedComponent
{
public:
    PricesTab();
    void init();
    ~PricesTab() override;
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    PricesJSON* pricesjson;
    PriceTab pTab[Core::MAX_PRICES];
    Disabled dis;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PricesTab)
};

/*
  ==============================================================================

    PriceTab.h
    Created: 19 Jun 2021 10:42:31am
    Author:  Eloi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class PricesTab  : public juce::TabbedComponent
{
public:
    PricesTab();
    ~PricesTab() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PricesTab)
};

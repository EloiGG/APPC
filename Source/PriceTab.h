/*
  ==============================================================================

    PriceTab.h
    Created: 21 Jun 2021 12:38:08pm
    Author:  admin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class PriceTab  : public juce::Component
{
public:
    PriceTab();
    ~PriceTab() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PriceTab)
};

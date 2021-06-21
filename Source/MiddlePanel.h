/*
  ==============================================================================

    MiddlePanel.h
    Created: 19 Jun 2021 1:07:43am
    Author:  Eloi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PriceDisplayComponent.h"
#include "LookAndFeel.h"
#include "Core.h"

//==============================================================================
/*
*/
class MiddlePanel  : public juce::Component
{
public:
    MiddlePanel();
    ~MiddlePanel() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void updatePrices(TextUpdateOrigin whocalled, unsigned int index);
private:
    PriceDisplayComponent prices;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MiddlePanel)
};

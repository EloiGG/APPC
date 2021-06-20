/*
  ==============================================================================

    RightPanel.h
    Created: 19 Jun 2021 1:07:30am
    Author:  Eloi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "LookAndFeel.h"
#include "GeneralTab.h"
#include "PricesTab.h"

//==============================================================================
/*
*/
class RightPanel  : public juce::TabbedComponent
{
public:
    RightPanel();
    ~RightPanel() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    PricesTab priceTab;
    GeneralTab genTab;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RightPanel)
};

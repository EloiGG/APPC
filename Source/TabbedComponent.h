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
class TabComponent  : public juce::TabbedComponent
{
public:
    TabComponent();
    ~TabComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void updateParameters();

private:
    PricesTab priceTab;
    GeneralTab genTab;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TabComponent)
};

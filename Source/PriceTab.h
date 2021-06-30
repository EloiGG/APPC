/*
  ==============================================================================

    PriceTab.h
    Created: 21 Jun 2021 12:38:08pm
    Author:  admin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Core.h"
#include "LookAndFeel.h"
#include "InputComponent.h"
#include "GridTool.h"
#include "JSON.h"
//==============================================================================
/*
*/
class PriceTab  : public juce::Component
{
public:
    PriceTab();
    ~PriceTab() override;
    void init(const PriceJSON& json);
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    InputComponent id, price, gasStation, fuel, created, updated, 
        position, emptied, modifyAt, modifyDone, status, oldPrice;
    GridTool grid;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PriceTab)
};

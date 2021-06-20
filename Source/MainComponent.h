#pragma once

#include <JuceHeader.h>
#include "MiddlePanel.h"
#include "TopPanel.h"
#include "RightPanel.h"
#include "LeftPanel.h"
#include "BottomPanel.h"


class MainComponent  : public juce::Component
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void updatePrices(TextUpdateOrigin whoCalled);

private:
    MiddlePanel mPanel;
    TopPanel tPanel;
    RightPanel rPanel;
    LeftPanel lPanel;
    BottomPanel bPanel;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

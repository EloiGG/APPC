#pragma once

#include <JuceHeader.h>
#include "MiddlePanel.h"
#include "TopPanel.h"
#include "RightPanel.h"
#include "LeftPanel.h"
#include "BottomPanel.h"
#include "Disabled.h"
#include "Log.h"
#include "Keyboard.h"
#include "GasStationSelection.h"
#include "PanelSelection.h"
class MainComponent : public juce::Component
{
public:
	//==============================================================================
	MainComponent();
	~MainComponent() override;

	//==============================================================================
	void paint(juce::Graphics&) override;
	void resized() override;

	void updatePrices(TextUpdateOrigin whoCalled, unsigned int index);

private:
	MiddlePanel mPanel;
	RightPanel rPanel;
	TooltipWindow tooltip;
	bool settingsOppened;
	GasStationSelectionDialogBox gasSelection;
	PanelSelectionDialogBox panelSelection;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

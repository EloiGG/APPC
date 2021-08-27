#pragma once

#include <JuceHeader.h>
#include "MiddlePanel.h"
#include "LeftPanel.h"
#include "Disabled.h"
#include "Log.h"
#include "Keyboard.h"
#include "GasStationSelection.h"
#include "PanelSelection.h"
#include "UCSelection.h"

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
	Keyboard kb;
	MiddlePanel mPanel;
	LeftPanel rPanel;
	TooltipWindow tooltip;
	bool settingsOppened;
	GasStationSelectionDialogBox gasSelection;
	PanelSelectionDialogBox panelSelection;
	UCSelectionDialogBox UCSelection;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

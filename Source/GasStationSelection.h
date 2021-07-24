/*
  ==============================================================================

	GasStationSelection.h
	Created: 20 Jul 2021 4:22:44pm
	Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "GridTool.h"
#include "LookAndFeel.h"   
#include "DialogBox.h"
#include "Core.h"
#include <vector>
//==============================================================================
/*
*/
class GasStationSelectionDialogBox : public DialogBoxComponent
{
	class GasStationSelection : public CustomTableListBox
	{
	public:
		GasStationSelection();
		~GasStationSelection() override;

		virtual void cellClicked(int rowNumber, int columnId, const MouseEvent&);
		virtual String getPropriety(int row, int column) override;

	private:
		const GasStationsJSON& stationsJSON;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GasStationSelection)
	};

public:
	GasStationSelectionDialogBox();

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GasStationSelectionDialogBox)
};
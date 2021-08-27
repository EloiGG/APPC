/*
  ==============================================================================

	GasStationSelection.h
	Created: 20 Jul 2021 4:22:44pm
	Author:  Eloi GUIHARD-GOUJON

	Contient la classe pour l'affichage de la liste des stations services

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

/// <summary>
/// Component affichant la liste des stations services
/// </summary>
class GasStationSelectionDialogBox : public DialogBoxComponent
{
	class GasStationSelection : public CustomTableListBox
	{
	public:
		GasStationSelection();
		void initialize();
		~GasStationSelection() override;
		virtual void cellClicked(int rowNumber, int columnId, const MouseEvent&);
		virtual String getPropriety(int row, int column) override;
		void paint(Graphics&) override;
	private:
		GasStationsJSON stationsJSON;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GasStationSelection)
	};

public:
	virtual void open() override;
	GasStationSelectionDialogBox();

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GasStationSelectionDialogBox)
};
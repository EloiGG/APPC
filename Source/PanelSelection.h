/*
  ==============================================================================

	PanelSelection.h
	Created: 21 Jul 2021 11:59:29am
	Author:  Eloi GUIHARD-GOUJON

	Contient la classe pour l'affichage de la liste des panneaux

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "DialogBox.h"
#include "GridTool.h"
#include "JSON.h"
#include "Core.h"

/// <summary>
/// Component affichant la liste des panneaux
/// </summary>
class PanelSelectionDialogBox : public DialogBoxComponent
{

	class PanelSelection : public CustomTableListBox
	{
	public:
		PanelSelection();

		virtual void cellClicked(int rowNumber, int columnId, const MouseEvent&) override;
		virtual String getPropriety(int row, int column) override;
		void paint(Graphics&) override;
	private:
		PanelJSON panelsJSON;
		int currentStationID;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PanelSelection)
	};

public:
	void open() override;
	PanelSelectionDialogBox();
private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PanelSelectionDialogBox)
};
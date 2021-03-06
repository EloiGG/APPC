
/*
  ==============================================================================

	PanelSelection.cpp
	Created: 21 Jul 2021 11:59:29am
	Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#include "PanelSelection.h"

void PanelSelectionDialogBox::open()
{
	DialogBoxComponent::open();
	Log::writeLn("Selection d'un panneau...");
}

PanelSelectionDialogBox::PanelSelectionDialogBox() : DialogBoxComponent(new PanelSelection)
{
	back.onClick = []()
	{
		Core::get().closeAllSelections();
		Core::get().selectGasStation();
		Core::get().updateVisualization();
	};
}

PanelSelectionDialogBox::PanelSelection::PanelSelection() : currentStationID(1)
{
	int w = 110;
	table.getHeader().addColumn("ID", 1, w);
	table.getHeader().addColumn("Nom", 2, w);
	table.getHeader().addColumn("Type", 3, w);
	table.getHeader().addColumn("Box", 4, w);
	table.setColour(juce::ListBox::outlineColourId, juce::Colours::grey);
	table.setOutlineThickness(1);

	table.autoSizeAllColumns();

	computeDesiredProportions();
}

void PanelSelectionDialogBox::PanelSelection::cellClicked(int rowNumber, int columnId, const MouseEvent&)
{
	Log::writeNext(" panneau selectionne : " + getPropriety(rowNumber, 2));
	Core::get().setCurrentPanelID(getPropriety(rowNumber, 1).getIntValue());
	Core::get().selectUC();
}

String PanelSelectionDialogBox::PanelSelection::getPropriety(int row, int column)
{
	String text;
	switch (column)
	{
	case 1:
		text = String(panelsJSON.getId());
		break;
	case 2:
		text = panelsJSON.getName();
		break;
	case 3:
		text = panelsJSON.getType();
		break;
	case 4:
		text = String(panelsJSON.getBox());
		break;
	default:
		break;
	}
	return text;
}

void PanelSelectionDialogBox::PanelSelection::paint(Graphics& g)
{
	g.fillAll(lfColours::panelBackground);
	if (Core::get().getCurrentStationID() != currentStationID) {
		bool success;
		const auto& s = Core::get().getPanelsjson(currentStationID, success);
		if (success) {
			init();
			panelsJSON = s;
			currentStationID = Core::get().getCurrentStationID();
			size = panelsJSON.getNumPanels();
			table.autoSizeAllColumns();

			computeDesiredProportions();
			getParentComponent()->resized();

		}
		else {
			Core::get().openAlertWindow(APPCAlertWindows::WindowType::NoConnection, "Impossible de charger les panneaux disponibles");
			size = 0;
		}
	}
}

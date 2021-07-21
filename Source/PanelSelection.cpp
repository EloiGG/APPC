
/*
  ==============================================================================

	PanelSelection.cpp
	Created: 21 Jul 2021 11:59:29am
	Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#include "PanelSelection.h"

PanelSelectionDialogBox::PanelSelectionDialogBox() : DialogBoxComponent(static_cast<Component*>(new PanelSelection))
{
	setInteriorProportions(0.7f, 0.7f);
}

PanelSelectionDialogBox::PanelSelection::PanelSelection()
{
	int w = 110;
	table.getHeader().addColumn("ID", 1, w);
	table.getHeader().addColumn("Nom", 2, w);
	table.getHeader().addColumn("Type", 3, w);
	table.getHeader().addColumn("Box", 4, w);
	table.autoSizeAllColumns();
	table.setColour(juce::ListBox::outlineColourId, juce::Colours::grey);
	table.setOutlineThickness(1);
}

void PanelSelectionDialogBox::PanelSelection::cellClicked(int rowNumber, int columnId, const MouseEvent&)
{
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
	default:
		break;
	}
	return text;
}

void PanelSelectionDialogBox::PanelSelection::paint(Graphics& g)
{
	g.fillAll(lfColours::panelBackground);
	if (Core::get().getCurrentStationID() != currentStationID) {
		currentStationID = Core::get().getCurrentStationID();
		panelsJSON = Core::get().getPanelsjson(currentStationID);
		size = panelsJSON.getNumPanels();
		table.autoSizeAllColumns();
	}
}

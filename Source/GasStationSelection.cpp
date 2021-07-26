/*
  ==============================================================================

	GasStationSelection.cpp
	Created: 20 Jul 2021 4:22:44pm
	Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#include <JuceHeader.h>
#include "GasStationSelection.h"

//==============================================================================
GasStationSelectionDialogBox::GasStationSelection::GasStationSelection() :
	stationsJSON(*Core::get().getGasStationsjson())
{
	size = Core::get().getGasStationsjson() ? stationsJSON.getNumGasStations() : 0;
	addAndMakeVisible(table);
	int w = 110;
	table.getHeader().addColumn("ID", 6, w);
	table.getHeader().addColumn("Nom", 1, w);
	table.getHeader().addColumn("Autoroute", 2, w);
	table.getHeader().addColumn("Direction", 3, w);
	table.getHeader().addColumn(L"Compagnie Pétrolière", 4, w);
	table.getHeader().addColumn(L"Compagnie Autoroutière", 5, w);   
	table.setOutlineThickness(1);

	table.autoSizeAllColumns();

	computeDesiredProportions();

}

GasStationSelectionDialogBox::GasStationSelection::~GasStationSelection()
{
}


void GasStationSelectionDialogBox::GasStationSelection::cellClicked(int rowNumber, int columnId, const MouseEvent&)
{
	Core::get().setCurrentStationID(getPropriety(rowNumber, 6).getIntValue());
	Core::get().selectPanel();
}

String GasStationSelectionDialogBox::GasStationSelection::getPropriety(int rowNumber, int columnId)
{
	String text;
	switch (columnId)
	{
	case 6:
		text = String(stationsJSON[rowNumber].getId());
		break;
	case 1:
		text = stationsJSON[rowNumber].getName();
		break;
	case 2:
		text = stationsJSON[rowNumber].getHighway();
		break;
	case 3:
		text = stationsJSON[rowNumber].getDirection();
		break;
	case 4:
		text = stationsJSON[rowNumber].getOilCompany();
		break;
	case 5:
		text = stationsJSON[rowNumber].getMotorwayCompany();
		break;
	default:
		break;
	}
	return text;
}

GasStationSelectionDialogBox::GasStationSelectionDialogBox() : DialogBoxComponent(new GasStationSelection)
{
	back.setEnabled(false);
}
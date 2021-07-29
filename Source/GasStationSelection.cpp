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
GasStationSelectionDialogBox::GasStationSelection::GasStationSelection()
{

	bool success;
	const String& s(Core::get().getGasStationsjson(success));
	if (success) {
		init();
		stationsJSON = s;
		stationsJSON.updatePropreties(Core::get().getNetwork());
		size = stationsJSON.getNumGasStations();
	}
	else
		size = 0;
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

void GasStationSelectionDialogBox::GasStationSelection::initialize()
{

}

GasStationSelectionDialogBox::GasStationSelection::~GasStationSelection()
{
}


void GasStationSelectionDialogBox::GasStationSelection::cellClicked(int rowNumber, int columnId, const MouseEvent&)
{
	Log::writeNext(" station selectionnee : " + getPropriety(rowNumber, 1));
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

void GasStationSelectionDialogBox::GasStationSelection::paint(Graphics&)
{
	if (!isInit()) {
		bool success;
		const String& s(Core::get().getGasStationsjson(success));
		if (success) {
			init();
			stationsJSON = s;
			size = stationsJSON.getNumGasStations();
			stationsJSON.updatePropreties(Core::get().getNetwork());
			table.updateContent();
			table.autoSizeAllColumns();
			computeDesiredProportions();
			getParentComponent()->resized();
		}
		else {
			Core::get().openAlertWindow(APPCAlertWindows::WindowType::NoConnection, "Impossible de charger les stations disponibles");
			size = 0;
		}
	}
}

void GasStationSelectionDialogBox::open()
{
	DialogBoxComponent::open();
	if (!insideComponent->isInit()) {
		insideComponent->repaint();
	}
	Log::writeLn("Selection d'une station service...");
}

GasStationSelectionDialogBox::GasStationSelectionDialogBox() : DialogBoxComponent(new GasStationSelection)
{
	back.setEnabled(false);
}
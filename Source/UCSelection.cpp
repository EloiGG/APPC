/*
  ==============================================================================

	UCSelection.cpp
	Created: 22 Jul 2021 3:47:05pm
	Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#include <JuceHeader.h>
#include "UCSelection.h"

//==============================================================================
UCSelectionDialogBox::UCSelection::UCSelection()
{
	int w = 110;
	table.getHeader().addColumn("ID", 1, w);
	table.getHeader().addColumn("Nom", 2, w);
	table.getHeader().addColumn(L"Avec contrôle", 3, w);
	table.getHeader().addColumn("Nombre de prix", 4, w);
	table.setColour(juce::ListBox::outlineColourId, juce::Colours::grey);
	table.setOutlineThickness(1);

	table.autoSizeAllColumns();

	computeDesiredProportions();
}

UCSelectionDialogBox::UCSelection::~UCSelection()
{
}


void UCSelectionDialogBox::UCSelection::cellClicked(int rowNumber, int columnId, const MouseEvent&)
{
	currentUCID = rowNumber;
	Log::writeNext(" UC selectionne : " + getPropriety(rowNumber, 2));
	Log::ln(1,2);

	Core::get().openAlertWindow(APPCAlertWindows::WindowType::PriceChoice, "", [this](int r)
		{

			const auto& uc = UCsjson[currentUCID];
			int numPrices = uc.getCountLinePrice();
			int numDigits = uc.getNumberOfDigits();
			int id = uc.getId();
			auto& c = Core::get();

			if (r == 1) Log::writeLn("Chargement du prix UC");
			if (r == 2) Log::writeLn("Chargement du prix de la station");

			String name(uc.getName());
			float prices[Core::MAX_PRICES] = { 0 };
			for (int i = 0; i < numPrices; i++) {
				if (r == 1) // prix UC 
					prices[uc.getUCLinePrice(i).getPosition()] = uc.getUCLinePrice(i).getPrice();
				if (r == 2) // prix station
					prices[uc.getUCLinePrice(i).getPosition()] = uc.getStationLinePrice(i).getPrice();
			}

			Log::writeLn("Parametres disponibles : ");
			if (name != "error") {
				c.setUCName(name);
				Log::writeLn("Nom UC");
			}
			if (numPrices != -1) {
				c.setNumPrices(numPrices);
				Log::writeLn("Nombre de prix");
			}
			if (numDigits != -1) {
				c.setNumDigits(numDigits);
				Log::writeLn("Nombre de chiffres par prix");
			}
			if (id != -1) {
				c.setID(id);
				Log::writeLn("ID Prix");
			}

			Log::ln();
			Log::writeLn("Chargement des parametres...");
			for (int i = 0; i < numPrices; i++)
				c.setPrice(i, String(prices[i]));

			c.closeUCSelection();
			for (int i = 0; i < Core::MAX_PRICES; i++)
				c.updatePrices(TextUpdateOrigin::Omni, i);
			c.resetInit();
			c.resetAllStates();
			c.updateVisualization();
			Log::writeLn("Parametres charges");
			Log::ln();

		});
}

String UCSelectionDialogBox::UCSelection::getPropriety(int row, int column)
{
	const auto& r = UCsjson[row];
	switch (column)
	{
	case 1:
		return String(r.getId());
	case 2:
		return r.getName();
	case 3:
		return r.getWithControl() ? "vrai" : "faux";
	case 4:
		return String(r.getCountLinePrice());
	}
}

void UCSelectionDialogBox::UCSelection::paint(Graphics& g)
{
	g.fillAll(lfColours::panelBackground);
	if (Core::get().getCurrentPanelID() != currentUCID) {
		bool success;
		currentUCID = Core::get().getCurrentPanelID();
		const String& s(Core::get().getUCsjson(currentUCID, success));
		if (success) {
			UCsjson = s;
			size = UCsjson.getNumUCJSON();
			table.autoSizeAllColumns();

			computeDesiredProportions();
			getParentComponent()->resized();
		}
		else {
			Core::get().openAlertWindow(APPCAlertWindows::WindowType::NoConnection, "Impossible de charger les UC disponibles");
			size = 0;
		}
	}
}

void UCSelectionDialogBox::open()
{
	DialogBoxComponent::open();
	Log::writeLn("Selection d'une UC...");
}

UCSelectionDialogBox::UCSelectionDialogBox() : DialogBoxComponent(new UCSelection)
{
	back.onClick = []()
	{
		Core::get().closeAllSelections();
		Core::get().selectPanel();
		Core::get().updateVisualization();
	};
}

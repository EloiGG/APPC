/*
  ==============================================================================

	GeneralTab.cpp
	Created: 19 Jun 2021 10:42:19am
	Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#include <JuceHeader.h>
#include "RightPanel.h"

//==============================================================================

RightPanel::RightPanel() : grid(4, 18), nPrices("Nombre de prix", Core::get().getNumPrices()), nDigits("Nombre de chiffres", Core::get().getNumDigits()),
resetLine("Effacer si erreur"), COM("Port COM", 3), centoFuelOpen("Choisir un panneau"), title(L"Paramètres", CharPointer_UTF8("Paramètres"))
{
	addChildComponent(grid);
	addAndMakeVisible(nPrices);
	addAndMakeVisible(nDigits);
	addAndMakeVisible(COM);
	addAndMakeVisible(resetLine);
	addAndMakeVisible(centoFuelOpen);
	addAndMakeVisible(title);
	addAndMakeVisible(disabled);

	title.setJustificationType(Justification::centred);

	disabled.setDisabled(true);

	grid.setBounds(0, 0, getWidth(), 300);

	nPrices.onUpdate = [](const String& input)
	{
		Core::get().setNumPrices(input.getIntValue());
		Core::get().resetInit();

		Core::get().updateVisualization();
	};
	nPrices.min = 1;
	nPrices.max = Core::MAX_PRICES;

	nDigits.onUpdate = [](const String& input)
	{
		Core::get().setNumDigits(input.getIntValue());
		Core::get().resetInit();
		Core::get().updateVisualization();
	};
	nDigits.min = 1;
	nDigits.max = Core::MAX_DIGITS;

	COM.onUpdate = [](const String& input)
	{
		Core::get().setCOMPort(input.getIntValue());
		Core::get().resetInit();
		Core::get().updateVisualization();
	};
	COM.min = 1;
	COM.max = 256;

	centoFuelOpen.setLookAndFeel(Core::get().getLookAndFeel().get());
	centoFuelOpen.setColour(TextButton::ColourIds::buttonColourId, lfColours::buttonBackground);
	centoFuelOpen.onClick = [this]()
	{
		if (Core::get().isConnected())
			Core::get().selectGasStation();
		else
		{
			Core::get().tryToConnect();
			if(Core::get().isConnected())
				Core::get().selectGasStation();
			else {
				Core::get().openAlertWindow(APPCAlertWindows::WindowType::NoConnection, [this](int r)
					{
						if (r == 2)
							centoFuelOpen.onClick();
					});
			}
		}
	};
}

RightPanel::~RightPanel()
{
}

void RightPanel::paint(juce::Graphics& g)
{
	g.fillAll(lfColours::tabBackground);
	g.setColour(lfColours::priceBackground.darker());
	g.fillRect(grid.getLine(0, false, 1.62f));
	g.setColour(Colours::black);
	g.drawRect(grid.getLine(0, false, 1.62f));
}

void RightPanel::resized()
{
	grid.setBounds(getLocalBounds());// .withWidth(getWidth()));
	setSize(grid.getWidth(), jmax(getHeight(), 600));
	title.setBounds(grid.getLine(0, false, 1.62f));
	COM.setBounds(grid.getLine(2));
	nPrices.setBounds(grid.getLine(3));
	nDigits.setBounds(grid.getLine(4));
	centoFuelOpen.setBounds(grid.getLine(6, false, 1.5f));
	disabled.setBounds(getLocalBounds());

	Font f(jmax<int>(title.getHeight() * 0.5, 16));
	title.setFont(f);
}

void RightPanel::setNumPrices(int newNumPrices)
{
	nPrices.setInput(String(newNumPrices));
}

void RightPanel::setNumDigits(int newNumDigits)
{
	nDigits.setInput(String(newNumDigits));
}

void RightPanel::updateParameters()
{
	auto& c = Core::get();
	setNumPrices(c.getNumPrices());
	setNumDigits(c.getNumDigits());
	setCOMPort(c.getCOMPort());
	resetLine.setToggleState(c.getResetLine(), NotificationType::sendNotification);
	if (c.getIsInTransmission())
		disabled.setDisabled(true);
	else
		disabled.setDisabled(false);
	repaint();
}

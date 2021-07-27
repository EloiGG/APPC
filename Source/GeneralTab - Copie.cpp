/*
  ==============================================================================

	GeneralTab.cpp
	Created: 19 Jun 2021 10:42:19am
	Author:  Eloi

  ==============================================================================
*/

#include <JuceHeader.h>
#include "GeneralTab.h"

//==============================================================================

GeneralTab::GeneralTab() : grid(4, 10), nPrices("Nombre de prix", Core::get().getNumPrices()), nDigits("Nombre de chiffres", Core::get().getNumDigits()),
resetLine("Effacer si erreur"), COM("Port COM", 3), centoFuelOpen("Choisir un panneau"), testProgression(testThread), stop("Stop")
{
	addChildComponent(grid);
	addAndMakeVisible(nPrices);
	addAndMakeVisible(nDigits);
	addAndMakeVisible(COM);
	addAndMakeVisible(resetLine);
	addAndMakeVisible(disabled);
	addAndMakeVisible(centoFuelOpen);
	addAndMakeVisible(stop);
	addAndMakeVisible(testProgression);
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
		/*if (testThread.isThreadRunning())
			return;

		if (Core::get().getPlaySequence() == false)
		{
			Sequence s;
			auto& c = Core::get();
			Price prices[Core::MAX_PRICES];
			for (int i = 0; i < Core::MAX_PRICES; i++)
				prices[i] = c.getPrice(i);
			s.createSequence(c.getNumPrices(), c.getNumDigits(), prices, c.getDelay(), c.getLineControl());
			testThread.setSequence(s);
		}
		else
			testThread.setSequence(Core::get().getSequence());
		stop.setEnabled(true);
		Core::get().setInTransmission(true);
		Core::get().updateVisualization();
		testProgression.start();
		testThread.startThread();*/
		Core::get().selectGasStation();
	};

	stop.setEnabled(false);
	stop.setLookAndFeel(Core::get().getLookAndFeel().get());
	stop.setColour(TextButton::ColourIds::buttonColourId, lfColours::stopButton);
	stop.onClick = [this]()
	{
		testThread.askToExit();
		Core::get().setInTesting(false);
		Core::get().updateVisualization();
		stop.setEnabled(false);
	};
}

GeneralTab::~GeneralTab()
{
	testThread.askToExit();
	testThread.waitForThreadToExit(3000);
}

void GeneralTab::paint(juce::Graphics& g)
{
	g.fillAll(lfColours::tabBackground);
}

void GeneralTab::resized()
{
	grid.setBounds(grid.getLocalBounds().withWidth(getWidth()));
	COM.setBounds(grid.getRectangle(0, 0, 4, 1));
	nPrices.setBounds(grid.getRectangle(0, 1, 4, 2));
	nDigits.setBounds(grid.getRectangle(0, 2, 4, 3));
	resetLine.setBounds(grid.getRectangle(0, 3, 4, 4));
	centoFuelOpen.setBounds(grid.getRectangle(0, 5.3, 4, 6.7));
	testProgression.setBounds(centoFuelOpen.getBounds());
	disabled.setBounds(getLocalBounds());
}

void GeneralTab::setNumPrices(int newNumPrices)
{
	nPrices.setInput(String(newNumPrices));
}

void GeneralTab::setNumDigits(int newNumDigits)
{
	nDigits.setInput(String(newNumDigits));
}

void GeneralTab::updateAllParameters()
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
	if (c.getInTesting())
		stop.setEnabled(true);
	else
		stop.setEnabled(false);
	repaint();
}

/*
  ==============================================================================

	DebugTab.cpp
	Created: 28 Jun 2021 3:24:13pm
	Author:  admin

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DebugTab.h"

//==============================================================================
DebugTab::DebugTab() : grid(4, 10), loadSequence(L"Charger une séquence"),
fileSelector(L"Charger une séquence", File::getCurrentWorkingDirectory().getParentDirectory(), "*.sequence"),
playSequenceCB(L"Jouer la séquence"),
checkPositions("Verifier"), blackout("Tout mettre au noir")
{
	addAndMakeVisible(grid);
	addAndMakeVisible(loadSequence);
	addAndMakeVisible(playSequenceCB);
	addAndMakeVisible(checkPositions);
	addAndMakeVisible(blackout);

	playSequenceCB.setToggleState(false, NotificationType::sendNotification);
	playSequenceCB.setEnabled(false);

	loadSequence.setLookAndFeel(Core::get().getLookAndFeel().get());
	blackout.setLookAndFeel(Core::get().getLookAndFeel().get());
	checkPositions.setLookAndFeel(Core::get().getLookAndFeel().get());

	loadSequence.setColour(TextButton::ColourIds::buttonColourId, lfColours::buttonBackground);
	blackout.setColour(TextButton::ColourIds::buttonColourId, lfColours::buttonBackground);
	checkPositions.setColour(TextButton::ColourIds::buttonColourId, lfColours::buttonBackground);

	loadSequence.onClick = [this]()
	{
		if (fileSelector.browseForFileToOpen()) {
			SequenceJSON json(fileSelector.getResult());
			Price* prices = new Price[json.getNumPrices()];
			Sequence seq;
			auto& c = Core::get();
			for (int i = 0; i < json.getSequenceSize(); i++) {
				for (int j = 0; j < json.getNumPrices(); j++)
					prices[j] = json.getPrice(i, j);
				seq += Sequence::getNewSequence(c.getNumPrices(), c.getNumDigits(), prices, c.getDelay(), c.getLineControl());
			}
			c.setSequence(seq);
			playSequenceCB.setToggleState(true, NotificationType::sendNotification);
			playSequenceCB.setEnabled(true);
			delete[] prices;
		}
	};
	playSequenceCB.onClick = [this]()
	{
		Core::get().setPlaySequence(playSequenceCB.getToggleState());
	};
	blackout.onClick = [this]()
	{
		auto& c = Core::get();
		Price p[Core::MAX_PRICES];
		for (int i = 0; i < Core::MAX_PRICES; i++) {
			p[i] = " .          ";
		c.setPrice(i, p[i]);
		c.updatePrices(TextUpdateOrigin::Omni, i);
		}
		sendThread.setSequence(Sequence::getNewSequence(c.getNumPrices(), c.getNumDigits(), p, 20, false));
		sendThread.setWaitForResponse(false);
		sendThread.startThread();
	};
}

DebugTab::~DebugTab()
{
	sendThread.askToExit();
	sendThread.stopThread(2000);
	sendThread.waitForThreadToExit(2000);
}

void DebugTab::paint(juce::Graphics& g)
{
	g.fillAll(lfColours::tabBackground);
}

void DebugTab::resized()
{
	grid.setBounds(getLocalBounds());
	loadSequence.setBounds(grid.getRectangle(1, 1, 3, 2));
	playSequenceCB.setBounds(grid.getRectangle(1, 2, 4, 3));

	checkPositions.setBounds(grid.getRectangle(1, 4, 3, 5));
	blackout.setBounds(grid.getRectangle(1, 5, 3, 6));
}

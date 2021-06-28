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
DebugTab::DebugTab() : grid(4, 10), loadSequence(CharPointer_UTF8("Charger une séquence")),
fileSelector(L"Charger une séquence", File::getCurrentWorkingDirectory().getParentDirectory(), "*.sequence"),
playSequenceCB(L"Jouer la séquence")
{
	addAndMakeVisible(grid);
	addAndMakeVisible(loadSequence);
	addAndMakeVisible(playSequenceCB);
	playSequenceCB.setToggleState(false, NotificationType::sendNotification);
	playSequenceCB.setEnabled(false);
	loadSequence.setLookAndFeel(Core::get().getLookAndFeel().get());
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
	playSequenceCB.onStateChange = [this]()
	{
		Core::get().setPlaySequence(playSequenceCB.getToggleState());
	};
}

DebugTab::~DebugTab()
{
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
}

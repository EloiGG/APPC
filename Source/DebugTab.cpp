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
allDigits(L"Afficher tous les chiffres à la suite"), blackout("Tout mettre au noir"), segmentsErrors(L"Détecter les erreurs segments")
{
	addAndMakeVisible(loadSequence);
	addAndMakeVisible(playSequenceCB);
	addAndMakeVisible(allDigits);
	addAndMakeVisible(blackout);
	addAndMakeVisible(segmentsErrors);

	playSequenceCB.setToggleState(false, NotificationType::sendNotification);
	playSequenceCB.setEnabled(false);

	loadSequence.setLookAndFeel(Core::get().getLookAndFeel().get());
	blackout.setLookAndFeel(Core::get().getLookAndFeel().get());
	allDigits.setLookAndFeel(Core::get().getLookAndFeel().get());
	segmentsErrors.setLookAndFeel(Core::get().getLookAndFeel().get());

	loadSequence.setColour(TextButton::ColourIds::buttonColourId, lfColours::buttonBackground);
	blackout.setColour(TextButton::ColourIds::buttonColourId, lfColours::buttonBackground);
	allDigits.setColour(TextButton::ColourIds::buttonColourId, lfColours::buttonBackground);
	segmentsErrors.setColour(TextButton::ColourIds::buttonColourId, lfColours::buttonBackground);

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
		if (c.getIsInTransmission())
			return;

		Price p[Core::MAX_PRICES];
		for (int i = 0; i < Core::MAX_PRICES; i++) {
			p[i] = " .          ";
			c.setPrice(i, p[i]);
			c.updatePrices(TextUpdateOrigin::Omni, i);
		}
		c.setSequence(Sequence::getNewSequence(c.getNumPrices(), c.getNumDigits(), p, 20, false));
		c.sendSequence();
	};
	allDigits.onClick = [this]()
	{
		auto& c = Core::get();
		if (c.getIsInTransmission())
			return;

		Sequence s(c.getNumDigits() * c.getNumPrices() * 11);
		s.setDelay(250);
		for (int prix = 0; prix < c.getNumPrices(); prix++)
			for (int digit = 0; digit < c.getNumDigits(); digit++)
				s.addStep(Sequence::SequenceStep(0x30 + prix * c.getNumDigits() + digit, 0x41, 0x0a));
		for (char character = '0'; character != '9' + 1; character++)
			for (int prix = 0; prix < c.getNumPrices(); prix++)
				for (int digit = 0; digit < c.getNumDigits(); digit++)
					s.addStep(Sequence::SequenceStep(0x30 + prix * c.getNumDigits() + digit, 0x41, character));

		c.setSequence(s);

		c.sendSequence();
	};
	segmentsErrors.onClick = [this]()
	{
		auto& c = Core::get();
		if (c.getIsInTransmission())
			return;

		Sequence s(c.getNumDigits() * c.getNumPrices());
		for (int i = 0; i < Core::MAX_PRICES; i++) {
			c.setPrice(i, Price("8.88888888"));
			c.updatePrices(TextUpdateOrigin::Omni, i);
		}
		for (int prix = 0; prix < c.getNumPrices(); prix++)
			for (int digit = 0; digit < c.getNumDigits(); digit++)
				s.addStep(Sequence::SequenceStep(0x30 + prix * c.getNumDigits() + digit, 0x46, '8'));

		c.setSequence(s);

		c.sendSequence();
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

	allDigits.setBounds(grid.getRectangle(1, 4, 3, 5));
	blackout.setBounds(grid.getRectangle(1, 5, 3, 6));
	segmentsErrors.setBounds(grid.getRectangle(1, 6, 3, 7));
}

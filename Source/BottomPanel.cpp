/*
  ==============================================================================

	BottomPanel.cpp
	Created: 19 Jun 2021 1:08:30am
	Author:  Eloi

  ==============================================================================
*/

#include <JuceHeader.h>
#include "BottomPanel.h"

//==============================================================================
BottomPanel::BottomPanel() : save("Sauvegarder"), details(L"Détails"),
filechooser("Sauvegarder le log", File::getCurrentWorkingDirectory(), "*.txt")
{
	addAndMakeVisible(t);
	t.setFont(Font(16));
	t.setMultiLine(true);
	t.setReturnKeyStartsNewLine(true);
	t.setColour(TextEditor::ColourIds::backgroundColourId, lfColours::priceBackground);
	t.setReadOnly(true);
	t.setScrollToShowCursor(true);

	addAndMakeVisible(save);
	save.setLookAndFeel(Core::get().getLookAndFeel().get());
	save.setColour(TextButton::ColourIds::buttonColourId, lfColours::buttonBackground);
	addAndMakeVisible(details);
	details.setToggleState(false, NotificationType::sendNotification);
	details.onClick = [this]()
	{
		debuggingLevel = details.getToggleState();
		t.moveCaretToEnd(false);
		resetText();
	};

	save.onClick = [this]()
	{
		filechooser.browseForFileToSave(true);
		File f(filechooser.getResult());
		f.create();
		f.replaceWithText(t.getText());
	};

	Log::get().updateFunction = [this]() {t.setText(Log::get().getString(debuggingLevel)); t.moveCaretToEnd(false); resetText(); };

	debuggingLevel = details.getToggleState();
	t.moveCaretToEnd(false);
	resetText();
}

BottomPanel::~BottomPanel()
{
}

void BottomPanel::paint(juce::Graphics& g)
{
	g.fillAll(lfColours::panelBackground);
}

void BottomPanel::resized()
{
	auto r = getLocalBounds();
	t.setBounds(r.removeFromLeft(getWidth() * 0.9));
	r.removeFromTop(getHeight() * 0.25);
	save.setBounds(r.removeFromTop(getHeight() * 0.25));
	details.setBounds(r.removeFromTop(getHeight() * 0.25));
}

void BottomPanel::resetText()
{
	t.setText(Log::get().getString(debuggingLevel));
}

TextEditor* BottomPanel::getTextEditor()
{
	return &t;
}


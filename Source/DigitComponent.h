/*
  ==============================================================================

	DigitComponent.h
	Created: 17 Jun 2021 11:04:03am
	Author:  admin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "LookAndFeel.h"
#include "Core.h"
#include "SpecialLabel.h"
#include "ErrorStateIndicator.h"
#include "SerialThread.h"
//==============================================================================
/*
*/

class DigitEditorLabel : public SpecialLabel
{
public:
	DigitEditorLabel() {
		setEditable(true, true, true);
	}
protected:
	virtual void editorShown(TextEditor* te) override
	{
		Core::get().showKeyboard((SpecialLabel*)this, te->getText(), 1);
	}
	virtual void editorAboutToBeHidden(TextEditor*) override
	{
		if (lastText.length() && lastText != getText()) {
			const auto& digit = getText().substring(0, 1);
			if (digit.containsOnly(Core::get().getDigitEditorAcceptedCharacters()))
				lastText = digit;
			setText(lastText, NotificationType::sendNotification);
			textManuallyUpdated();
		}
	}
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DigitEditorLabel)
};

class DigitEditor : public DigitEditorLabel, private Timer
{
public:
	DigitEditor();

	void paint(juce::Graphics&) override;
	void setDigit(const String& newDigit);
	String getDigit();
	void resized() override;
	void setState(const ErrModule& newState);
	ErrModule getState() { return state; }
	void setShowState(bool shouldShowState);
	void updateAnimation();

private:
	bool hasState;
	virtual void timerCallback() override;
	unsigned int frameCounter;
	ErrorStateIndicator stateIndicator;
	ErrModule state;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DigitEditor)
};

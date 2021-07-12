/*
  ==============================================================================

	SpecialLabel.h
	Created: 23 Jun 2021 9:27:16am
	Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <functional>

class SpecialLabel : public Label
{
public:
	SpecialLabel(const std::function<void()>& onTextUpdate) : textManuallyUpdated(onTextUpdate), lastText("") { }
	SpecialLabel() : lastText("") {}
	std::function<void()> textManuallyUpdated;

protected:
	virtual void editorAboutToBeHidden(TextEditor*) override
	{
		if (lastText != getText()) {
			textManuallyUpdated();
			lastText = getText();
		}
	}
	String lastText;
};

class DigitEdiorLabel : public SpecialLabel
{
public:
	DigitEdiorLabel() {}
protected:
	virtual void editorAboutToBeHidden(TextEditor*) override
	{
		if (lastText != getText()) {
			const auto& digit = getText().substring(0, 1);
			if (digit.containsOnly(Core::get().getDigitEditorAcceptedCharacters()))
				lastText = digit;
			setText(lastText, NotificationType::sendNotification);
			textManuallyUpdated();
		}
	}
};
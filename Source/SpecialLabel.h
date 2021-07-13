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

class DigitEditorLabel : public SpecialLabel
{
public:
	DigitEditorLabel() {
		setEditable(true, true, true);
	}
protected:
	virtual void editorShown(TextEditor*) override
	{
		Core::get().showKeyboard(getBounds(), 1);
	}
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

class PriceEditorLabel : public SpecialLabel
{
public:
	PriceEditorLabel() {}
protected:

	virtual void editorAboutToBeHidden(TextEditor*) override
	{
		if (lastText != getText()) {
			const auto& str = getText();
			if (str.containsOnly(Core::get().getDigitEditorAcceptedCharacters()))
				lastText = str;
			setText(lastText, NotificationType::sendNotification);
			if (textManuallyUpdated)
				textManuallyUpdated();
		}
	}
};
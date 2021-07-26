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
			if (textManuallyUpdated)
				textManuallyUpdated();
			lastText = getText();
		}
	}
	String lastText;

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpecialLabel)
};

class KeyboardLabel : public SpecialLabel
{
public:
	KeyboardLabel() = default;
	void callForUpdate() { textKeyboardUpdated(); if (onTextKeyboardUpdate) onTextKeyboardUpdate(); }
	std::function<void()> onTextKeyboardUpdate;
protected:
	virtual void textKeyboardUpdated() {}
	virtual void editorAboutToBeHidden(TextEditor*) override {}
private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KeyboardLabel)
};
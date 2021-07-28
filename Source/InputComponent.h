/*
  ==============================================================================

	InputComponent.h
	Created: 21 Jun 2021 2:24:45pm
	Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Core.h"
#include "SpecialLabel.h"
#include "Log.h"

//==============================================================================
/*
*/
class InputComponentLabel : public KeyboardLabel
{
public:
	InputComponentLabel() : maxNumChar(1) {}
	InputComponentLabel(const std::function<void()>& f) : maxNumChar(1)
	{
		textManuallyUpdated = f;
		onTextKeyboardUpdate = f;
	}
	unsigned int maxNumChar;
protected:
	//virtual void textKeyboardUpdated() {}
	//virtual void editorAboutToBeHidden(TextEditor*) override {}
	virtual void editorShown(TextEditor* te) override
	{
		Core::get().showKeyboard(this, [](const String& output)
			{
				return output.containsOnly("0987654321");
			},
			te->getText(), maxNumChar);
	}
private:
};

class InputComponent : public juce::Component
{
public:
	InputComponent(const String& parameterName, const String& defaultValue = "");
	InputComponent(const String& parameterName, const int& defaultValue);
	~InputComponent() override;

	void paint(juce::Graphics&) override;
	void resized() override;

	void setTitle(const String& newTitle) { title.setText(newTitle, NotificationType::sendNotification); }
	void setInput(const String& newInput) { input.setText(newInput, NotificationType::sendNotification); }
	void setMaxInputLengh(unsigned int maxLengh) { input.maxNumChar = maxLengh; }

	std::function<void()>& onIncrement;
	std::function<void()>& onDecrement;
	std::function<void(const String&)> onUpdate;

	float min, max;
	int increment, decrement;

private:
	String lastText;
	Label title;
	InputComponentLabel input;
	TextButton p, m;
	int titleWidth;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InputComponent)
};

class CheckBox : public ToggleButton
{
public:
	CheckBox(const String& text);
private:
	int titleWidth;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CheckBox)
};
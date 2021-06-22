/*
  ==============================================================================

	InputComponent.cpp
	Created: 21 Jun 2021 2:24:45pm
	Author:  admin

  ==============================================================================
*/

#include <JuceHeader.h>
#include "InputComponent.h"

//==============================================================================
InputComponent::InputComponent(const String& parameterName, const String& defaultValue) : title(parameterName, parameterName + String(" : ")), input(defaultValue, defaultValue),
p("+"), m("-"), onIncrement(p.onClick), onDecrement(m.onClick), min(0), max(10), lastText(defaultValue)
{
	p.setLookAndFeel(Core::get().getLookAndFeel().get());
	m.setLookAndFeel(Core::get().getLookAndFeel().get());
	p.setColour(TextButton::ColourIds::buttonColourId, lfColours::buttonBackground);
	m.setColour(TextButton::ColourIds::buttonColourId, lfColours::buttonBackground);
	titleWidth = title.getText().length() * 8;
	input.setEditable(true);

	addAndMakeVisible(title);
	addAndMakeVisible(input);
	addAndMakeVisible(p);
	addAndMakeVisible(m);

	onIncrement = [this]()
	{
		auto n = input.getText(true).getIntValue() + 1;
		input.setText(String(n), NotificationType::sendNotification);
	};

	onDecrement = [this]()
	{
		auto n = input.getText(true).getIntValue() - 1;
		input.setText(String(n), NotificationType::sendNotification);
	};

	input.onTextChange = [this]()
	{
		auto n = input.getText(true).getFloatValue();
		if (n <= max && n >= min) {
			lastText = input.getText();
			if (onUpdate)
				onUpdate(lastText);
		}
		else
			input.setText(lastText, NotificationType::sendNotification);
	};

}

InputComponent::InputComponent(const String& parameterName, const int& defaultValue) : InputComponent(parameterName, String(defaultValue))
{
}

InputComponent::~InputComponent()
{
}

void InputComponent::paint(juce::Graphics& g)
{
	g.setColour(lfColours::inputBackground);
	g.fillRoundedRectangle(input.getBounds().toFloat(), 3.0f);

	g.setColour(lfColours::inputOutline.withAlpha(0.5f));
	g.drawRect(input.getBounds());
	g.drawRect(p.getBounds());
	g.drawRect(m.getBounds());
}

void InputComponent::resized()
{
	float inputwidth = 0.75;
	auto r = getLocalBounds();
	title.setBounds(r.removeFromLeft(jmin<int>(titleWidth, getWidth() * 0.6)));
	auto s = r.removeFromRight(30);
	p.setBounds(s.removeFromTop(getHeight() * 0.5));
	m.setBounds(s);
	input.setBounds(r);
}

CheckBox::CheckBox(const String& text) : ToggleButton(text+ String(" : "))
{
	setLookAndFeel(Core::get().getLookAndFeel().get());
}

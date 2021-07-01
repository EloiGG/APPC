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
InputComponent::InputComponent(const String& parameterName, const String& defaultValue) : title(parameterName, parameterName + String(" : ")),
p("+"), m("-"), onIncrement(p.onClick), onDecrement(m.onClick), min(0), max(10), lastText(defaultValue), increment(1), decrement(1),
input([this]()
	{
		auto n = input.getText(true).getFloatValue();
		if (n <= max && n >= min) {
			lastText = input.getText();
			if (onUpdate)
				onUpdate(lastText);
		}
		else
			input.setText(lastText, NotificationType::sendNotification);
	})
{
	p.setLookAndFeel(Core::get().getLookAndFeel().get());
	m.setLookAndFeel(Core::get().getLookAndFeel().get());
	p.setColour(TextButton::ColourIds::buttonColourId, lfColours::buttonBackground);
	m.setColour(TextButton::ColourIds::buttonColourId, lfColours::buttonBackground);
	titleWidth = title.getFont().getStringWidth(title.getText());
	input.setEditable(true);

	addAndMakeVisible(title);
	addAndMakeVisible(input);
	addAndMakeVisible(p);
	addAndMakeVisible(m);

	input.setText(String(defaultValue), NotificationType::sendNotification);

	onIncrement = [this]()
	{
		int intval = input.getText(true).getIntValue();
		auto n = increment == 1 ? intval + 1 :
			intval % increment == 0 ? intval + increment : (intval + increment) / increment * increment;
		input.setText(String(n), NotificationType::sendNotification);
		input.textManuallyUpdated();
	};

	onDecrement = [this]()
	{
		int intval = input.getText(true).getIntValue();
		auto n = decrement == 1 ? intval - 1 :
			intval % decrement == 0 ? intval - decrement : (intval - decrement) / decrement * decrement;
		input.setText(String(n), NotificationType::sendNotification);
		input.textManuallyUpdated();
	};

	input.onTextChange = [this]()
	{
		Log::write(L"Changement du paramètre \"" + String(title.getText().substring(0, title.getText().length() - 3)) + L"\". \tNouvelle valeur : " + String(input.getText()), 2);
		Log::ln(2);
		Log::update();
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

	CheckBox::CheckBox(const String& text) : ToggleButton(text + String(" : "))
	{
		setLookAndFeel(Core::get().getLookAndFeel().get());
		onClick = [this]() {
			Log::write(L"Changement d'état du paramètre \"", 2);
			Log::write(getButtonText().substring(0, getButtonText().length() - 3), 2);
			Log::write(L"\". Nouvel état : ", 2);
			Log::write(String((int)getToggleState()), 2);
			Log::ln(2);
			Log::update();
		};
	}

	ConstInputComponent::ConstInputComponent(const String& parameterName, const String& value) : 
		title(parameterName, parameterName + String(" : "))
	{
		titleWidth = title.getFont().getStringWidth(title.getText());
		input.setEditable(false);

		addAndMakeVisible(title);
		addAndMakeVisible(input);

		input.setText(String(value), NotificationType::sendNotification);
	}

	void ConstInputComponent::paint(juce::Graphics& g)
	{
		g.setColour(lfColours::inputBackground);
		g.fillRoundedRectangle(input.getBounds().toFloat(), 3.0f);

		g.setColour(lfColours::inputOutline.withAlpha(0.5f));
		g.drawRect(input.getBounds());
	}

	void ConstInputComponent::resized()
	{
		float inputwidth = 0.75;
		auto r = getLocalBounds();
		title.setBounds(r.removeFromLeft(jmin<int>(titleWidth, getWidth() * 0.8)));
		input.setBounds(r);
	}

/*
  ==============================================================================

	Disabled.h
	Created: 24 Jun 2021 11:41:40am
	Author:  Eloi GUIHARD-GOUJON

	Contient de quoi marquer un component comme désactivé/inaccessible

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/

/// <summary>
/// Component qui permet de rayer une zone pour la marquer désactivée/inaccessible
/// </summary>
class Disabled : public juce::Component
{
public:
	Disabled() : isDisabled(false)
	{
		setInterceptsMouseClicks(false, false);
	}

	~Disabled() override
	{
	}

	void paint(juce::Graphics& g) override
	{
		if (!isDisabled)
			return;
		g.fillAll(Colours::grey.withAlpha(0.45f));
		g.setColour(Colours::black);
		int spacing = 7, w = 0, h = 0;
		for (int y = 0; y < getHeight() + getWidth(); y += spacing) {
			g.drawLine(Line<float>(0, y, y, 0), 1.0f);
		}
		g.setColour(Colours::white);
		g.drawText(message, getLocalBounds(), Justification::centred);
	}

	void resized() override
	{
	}

	// Marque la zone comme désactivée, la hachurant
	void setDisabled(bool shouldBeDisabled)
	{
		isDisabled = shouldBeDisabled;
		setInterceptsMouseClicks(shouldBeDisabled, shouldBeDisabled);
	}

	void setMessage(const String& newMessage)
	{
		message = newMessage;
	}

private:
	String message;
	bool isDisabled;
	Path p, c;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Disabled)
};

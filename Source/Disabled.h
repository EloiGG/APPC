/*
  ==============================================================================

	Disabled.h
	Created: 24 Jun 2021 11:41:40am
	Author:  admin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
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
		/*for (w = 0; w + getHeight() <= getWidth(); w += spacing)
			g.drawLine(w, 0, w + getHeight(), getHeight());
		for (h = 0; h + getWidth() <= getHeight(); h += spacing)
			g.drawLine(0, h, getWidth(), getWidth() + h);
		for (; w < getWidth(); w += spacing)
			g.drawLine(w, 0, getWidth(), getWidth() - w);
		for (h += spacing; h < getHeight(); h += spacing)
			g.drawLine(0, h, getHeight() - h, getHeight());*/
		for (int y = 0; y < getHeight() + getWidth(); y += spacing) {
			g.drawLine(Line<float>(0, y, y, 0), 1.0f);
		}
		g.setColour(Colours::white);
		g.drawText(message, getLocalBounds(), Justification::centred);
	}

	void resized() override
	{
	}
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

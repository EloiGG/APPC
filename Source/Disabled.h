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
	}

	~Disabled() override
	{
	}

	void paint(juce::Graphics& g) override
	{
		g.fillAll(Colours::grey.withAlpha(0.3f));
		g.setColour(Colours::black);
		int spacing = 5, w = 0, h = 0;
		for (w = 0; w + getHeight() <= getWidth(); w += spacing) 
			g.drawLine(w, 0, w + getHeight(), getHeight());
		for (h = 0; h + getWidth() <= getHeight(); h += spacing) 
			g.drawLine(0, h, getWidth(), getWidth() + h);
		for (; w < getWidth(); w += spacing) {
			g.drawLine(w, 0, getWidth(), getWidth() - w);
			g.drawLine(0, getHeight() + w - getWidth(), getWidth() - w, getHeight());
		}
	}

	void resized() override
	{
	}
	void setDisabled(bool shouldBeDisabled)
	{
		isDisabled = shouldBeDisabled;
	}
private:
	bool isDisabled;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Disabled)
};

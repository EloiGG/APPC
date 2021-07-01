/*
  ==============================================================================

	LookAndFeel.cpp
	Created: 17 Jun 2021 11:04:18am
	Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#include "LookAndFeel.h"

void APPCLookAndFeel::drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
	auto cornerSize = 0;
	auto bounds = button.getLocalBounds().toFloat().reduced(0.5f, 0.5f);

	auto baseColour = backgroundColour;



	if (shouldDrawButtonAsDown || shouldDrawButtonAsHighlighted)
		baseColour = baseColour.contrasting(shouldDrawButtonAsDown ? 0.2f : 0.05f);

	g.setColour(baseColour);

	g.fillRoundedRectangle(bounds, cornerSize);


	if (!button.isEnabled())
	{
		g.fillAll(Colours::grey.withAlpha(0.3f));
		g.setColour(Colours::black);
		int spacing = 5, w = 0, h = 0;
		for (w = 0; w + bounds.getHeight() <= bounds.getWidth(); w += spacing)
			g.drawLine(w, 0, w + bounds.getHeight(), bounds.getHeight());
		for (h = 0; h + bounds.getWidth() <= bounds.getHeight(); h += spacing)
			g.drawLine(0, h, bounds.getWidth(), bounds.getWidth() + h);
		for (; w < bounds.getWidth(); w += spacing) 
			g.drawLine(w, 0, bounds.getWidth(), bounds.getWidth() - w);
		for (h+=spacing; h < bounds.getHeight(); h += spacing)
			g.drawLine(0, h, bounds.getHeight() - h, bounds.getHeight());
	}

	g.setColour(button.findColour(ComboBox::outlineColourId));
	g.drawRoundedRectangle(bounds, cornerSize, 1.0f);
}

Font APPCLookAndFeel::getTextButtonFont(TextButton&, int buttonHeight)
{
	return { jmin(16.0f, (float)buttonHeight * 0.9f) };
}

void APPCLookAndFeel::drawToggleButton(Graphics& g, ToggleButton& button,
	bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
	auto fontSize = jmin(15.0f, (float)button.getHeight() * 0.75f);
	auto tickWidth = fontSize * 1.1f;
	Font f(g.getCurrentFont());
	f.setHeight(fontSize);
	auto titleWidth  = f.getStringWidth(button.getButtonText());

	

	g.setColour(button.findColour(ToggleButton::textColourId));
	g.setFont(fontSize);

	if (!button.isEnabled())
		g.setOpacity(0.5f);
	auto r = button.getLocalBounds();
	float inputwidth = 1;

	g.drawFittedText(button.getButtonText(), r.removeFromLeft(jmin(jmax<int>(titleWidth, r.getWidth() * 0.6), int(r.getWidth() * 0.8)))
		.withTrimmedTop(1).withTrimmedLeft(5).withTrimmedBottom(2).withTrimmedRight(5), Justification::left, 10);

	g.setColour(Colours::white);
	drawTickBox(g, button, r.getPosition().x, r.getPosition().y + (button.getHeight() - tickWidth) / 2,
		tickWidth, tickWidth,
		button.getToggleState(),
		button.isEnabled(),
		shouldDrawButtonAsHighlighted,
		shouldDrawButtonAsDown);
}

void APPCLookAndFeel::drawTickBox(Graphics& g, Component& component,
	float x, float y, float w, float h,
	const bool ticked,
	const bool isEnabled,
	const bool shouldDrawButtonAsHighlighted,
	const bool shouldDrawButtonAsDown)
{
	ignoreUnused(isEnabled, shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);

	Rectangle<float> tickBounds(x, y, w, h);

	g.setColour(component.findColour(ToggleButton::tickDisabledColourId));
	g.drawRect(tickBounds);

	if (ticked)
	{
		g.setColour(component.findColour(ToggleButton::tickColourId));
		auto tick = getTickShape(0.75f);
		g.fillPath(tick, tick.getTransformToScaleToFit(tickBounds.reduced(4, 5).toFloat(), false));
	}
}
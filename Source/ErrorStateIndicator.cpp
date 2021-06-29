/*
  ==============================================================================

	ErrorStateIndicator.cpp
	Created: 29 Jun 2021 12:03:05pm
	Author:  admin

  ==============================================================================
*/

#include "ErrorStateIndicator.h"

ErrorStateIndicator::ErrorStateIndicator() 
{
	state.err_ok = true;
	setInterceptsMouseClicks(true, false);
	setTooltip("CECI EST UN TEST");
}

ErrorStateIndicator::~ErrorStateIndicator()
{
}

void ErrorStateIndicator::setState(const ErrModule& newState)
{
	state = newState;
}

void ErrorStateIndicator::paint(Graphics& g)
{
	const float radius = jmin(getWidth(), getHeight()) * 0.2;
	int offset = 10;
	g.setColour(Colours::green);
	g.fillEllipse(getWidth() -  radius - offset, offset, radius, radius);
}

void ErrorStateIndicator::resized()
{
	setBounds(getLocalBounds());
}


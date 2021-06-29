/*
  ==============================================================================

    ErrorStateIndicator.h
    Created: 29 Jun 2021 12:03:05pm
    Author:  admin

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "SerialThread.h"
class ErrorStateIndicator :  public juce::SettableTooltipClient, public Component
{
public:
    explicit ErrorStateIndicator();
    ~ErrorStateIndicator() override;
    void setState(const ErrModule& newState);
    void paint(Graphics&) override;
    void resized() override;
private:
    ErrModule state;
};
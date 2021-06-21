/*
  ==============================================================================

    InputComponent.h
    Created: 21 Jun 2021 2:24:45pm
    Author:  admin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Core.h"

//==============================================================================
/*
*/
class InputComponent  : public juce::Component
{
public:
    InputComponent(const String& parameterName, const String& defaultValue = "");
    InputComponent(const String& parameterName, const int& defaultValue);
    ~InputComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    std::function<void()>& onIncrement;
    std::function<void()>& onDecrement;
    std::function<void(String&)> onUpdate;

    float min, max;

private:
    String lastText;
    Label title, input;
    TextButton p, m;
    int titleWidth;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InputComponent)
};

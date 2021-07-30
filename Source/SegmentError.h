/*
  ==============================================================================

    SegmentError.h
    Created: 30 Jul 2021 8:37:52am
    Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class SegmentError  : public juce::Component
{
public:
    SegmentError();
    ~SegmentError() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    Path segments[7];
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SegmentError)
};

/*
  ==============================================================================

    SegmentError.cpp
    Created: 30 Jul 2021 8:37:52am
    Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SegmentError.h"

//==============================================================================
SegmentError::SegmentError()
{

}

SegmentError::~SegmentError()
{
}

void SegmentError::paint (juce::Graphics& g)
{

}

void SegmentError::resized()
{
    
    segments[0].startNewSubPath(getBounds().getCentre().toFloat());

}

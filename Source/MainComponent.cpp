#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize (500, 400);
    addAndMakeVisible(prices);
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    prices.setBounds(getLocalBounds());
}

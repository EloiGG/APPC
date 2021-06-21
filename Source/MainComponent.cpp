#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
	Core::get().setUpdatePriceFunction(
		[this](TextUpdateOrigin o, unsigned int index)
		{
			mPanel.updatePrices(o, index);
		}
	);
	addAndMakeVisible(lPanel);
	addAndMakeVisible(mPanel);
	addAndMakeVisible(rPanel);
	addAndMakeVisible(tPanel);
	addAndMakeVisible(bPanel);
	setSize(1000, 600);
	auto o = Core::get().getPrice(0);
	int i;
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint(juce::Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
	int w = getWidth(), h = getHeight();
	float topHeight = 0.08f * h, middleWidth = 0.6f * w, leftWidth = 0.15f * w, bottomHeight = 0.3f * h;
	auto bounds = getLocalBounds();
	tPanel.setBounds(bounds.removeFromTop(topHeight));
	bPanel.setBounds(bounds.removeFromBottom(bottomHeight));
	//lPanel.setBounds(bounds.removeFromLeft(leftWidth));
	mPanel.setBounds(bounds.removeFromLeft(middleWidth));
	rPanel.setBounds(bounds);
}

void MainComponent::updatePrices(TextUpdateOrigin whoCalled, unsigned int index)
{
	mPanel.updatePrices(whoCalled, index);
}

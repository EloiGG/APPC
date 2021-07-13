#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : tooltip(this, 200), settingsOppened(false)
{
	Core::get().setUpdatePriceFunction(
		[this](TextUpdateOrigin o, unsigned int index)
		{
			mPanel.updatePrices(o, index);
			Log::write("Changement du prix ");
			Log::write(String(index + 1));
			Log::write(". Nouveau prix : ");
			Log::write(Core::get().getPrice(index).toString(Core::MAX_DIGITS));
			Log::ln();
			Log::update();
		}
	);

	Core::get().updateVisualization = [this]()
	{
		mPanel.updateVisualization();
		rPanel.updateParameters();
		Log::update();
	};

	Core::get().openSettings = [this]()
	{
		settingsOppened = true;
		rPanel.setVisible(true);
		setSize(getWidth() * 1.0 / 0.6, getHeight());
	};

	Core::get().closeSettings = [this]()
	{
		settingsOppened = false;
		rPanel.setVisible(false);
		setSize(getWidth() * 0.6, getHeight());
	};

	addAndMakeVisible(mPanel);
	addAndMakeVisible(rPanel);
	addAndMakeVisible(tooltip);

	Core::get().updateVisualization();

	setSize(600, 600);
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
	//tPanel.setBounds(bounds.removeFromTop(topHeight));
	//bPanel.setBounds(bounds.removeFromBottom(bottomHeight));
	//lPanel.setBounds(bounds.removeFromLeft(leftWidth));
	if(settingsOppened) rPanel.setBounds(bounds.removeFromRight(w * 0.4));
	mPanel.setBounds(bounds);
	tooltip.setBounds(getLocalBounds());
	//mPanel.setBounds(getLocalBounds());
}

void MainComponent::updatePrices(TextUpdateOrigin whoCalled, unsigned int index)
{
	mPanel.updatePrices(whoCalled, index);
}

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : tooltip(this, 200), settingsOppened(false)
{
	gasSelection.close();
	panelSelection.close();
	auto& c = Core::get();

	c.setCurrentStationID(2);
	c.selectGasStation = [this]()
	{
		gasSelection.open();
	};
	c.selectPanel = [this]()
	{
		gasSelection.close();
		panelSelection.open();
	};
	c.selectUC = [this]()
	{
		panelSelection.close();
		UCSelection.open();
	};
	c.closeUCSelection = [this]()
	{
		UCSelection.close();
	};
	c.setUpdatePriceFunction(
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

	c.updateVisualization = [this]()
	{
		mPanel.updateVisualization();
		rPanel.updateParameters();
		Log::update();
	};

	c.openSettings = [this]()
	{
		settingsOppened = true;
		rPanel.setVisible(true);
		setSize(getWidth() * 1.0 / 0.6, getHeight());
	};

	c.closeSettings = [this]()
	{
		settingsOppened = false;
		rPanel.setVisible(false);
		setSize(getWidth() * 0.6, getHeight());
	};

	addAndMakeVisible(mPanel);
	addAndMakeVisible(rPanel);
	addAndMakeVisible(tooltip);
	addAndMakeVisible(gasSelection);
	addAndMakeVisible(panelSelection);
	addAndMakeVisible(UCSelection);

	c.setCurrentPanelID(1);
	c.updateVisualization();

	setSize(600, 600);

	if (Core::get().isConnected())
		c.openAlertWindow(APPCAlertWindows::WindowType::LoadFromCentoFuel, [this](int r)
			{
				if (r == 1)
					gasSelection.open();
			}
	);
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
	if (settingsOppened) rPanel.setBounds(bounds.removeFromRight(w * 0.4));
	mPanel.setBounds(bounds);
	tooltip.setBounds(getLocalBounds());
	gasSelection.setBounds(getLocalBounds());
	panelSelection.setBounds(getLocalBounds());
	UCSelection.setBounds(getLocalBounds());
}

void MainComponent::updatePrices(TextUpdateOrigin whoCalled, unsigned int index)
{
	mPanel.updatePrices(whoCalled, index);
}

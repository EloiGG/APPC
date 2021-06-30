/*
  ==============================================================================

	PriceTab.cpp
	Created: 19 Jun 2021 10:42:31am
	Author:  Eloi

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PricesTab.h"

//==============================================================================
PricesTab::PricesTab() : TabbedComponent(TabbedButtonBar::Orientation::TabsAtTop), pricesjson(nullptr)
{
	addAndMakeVisible(dis);
	init();
	dis.setMessage(L"Il faut se connecter à CentoFuel pour accéder à cet onglet");
}

void PricesTab::init()
{
	if (!Core::get().isConnected()) {
		clearTabs();
		dis.setDisabled(true);
		return;
	}
	dis.setDisabled(false);
	
	if (pricesjson != Core::get().getpricesjson() && Core::get().getpricesjson() != nullptr) {
		clearTabs();
		pricesjson = Core::get().getpricesjson();
		for (int i = 0; i < pricesjson->getNumPrices(); i++) {
			pTab[i].init(pricesjson->operator[](i));
			String tabname = String("Prix ") + String(1 + i);
			addTab(tabname, lfColours::tabBackground, &pTab[i], false);
		}
	}

}

PricesTab::~PricesTab()
{
}

void PricesTab::paint(juce::Graphics& g)
{
}

void PricesTab::resized()
{
	TabbedComponent::resized();
	dis.setBounds(getLocalBounds());
}

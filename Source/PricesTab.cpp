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
	init();
	
}

void PricesTab::init()
{
	if (pricesjson != Core::get().getpricesjson() && Core::get().getpricesjson() != nullptr) {
		pricesjson = Core::get().getpricesjson();
		for (int i = 0; i < pricesjson->getNumPrices(); i++)
			pTab[i].init(pricesjson->operator[](i));
	}

	clearTabs();
	for (int i = 0; i < Core::get().getNumPrices(); i++) {
		String tabname = String("Prix ") + String(1 + i);
		addTab(tabname, lfColours::tabBackground, &pTab[i], false);
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
}

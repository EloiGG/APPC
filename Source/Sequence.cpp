/*
  ==============================================================================

	Sequence.cpp
	Created: 23 Jun 2021 11:02:25am
	Author:  admin

  ==============================================================================
*/

#include "Sequence.h"

Sequence::Sequence()
{
	v.reserve(16);
}

void Sequence::addStep(const SequenceStep& newStep)
{
	v.push_back(newStep);
	std::vector<SequenceStep> w;
}

void Sequence::createSequence(const unsigned int& numPrices, const unsigned int& numDigits,
	Price* prices, const unsigned int& delay_in_milliseconds, const bool& lineControl)
{
	v.resize(0);
	unsigned char c = lineControl ? 0x46 : 0x41;
	for (int prix = 0; prix < numPrices; prix++) {
		for (int chiffre = 0; chiffre < numDigits; chiffre++) {
			v.push_back({ unsigned char(0x30 + prix * numDigits + chiffre) , c, (unsigned char)prices[prix][chiffre][0] });
			DBG((unsigned char)prices[prix][chiffre][0]);
		}
	}
}


void Sequence::createSequence(const Core& core)
{
	auto& c = core.get();
	Price prices[Core::MAX_PRICES];
	for (int i = 0; i < c.getNumPrices(); i++)
		prices[i] = c.getPrice(i);
	createSequence(c.getNumPrices(), c.getNumDigits(), prices, c.getDelay(), c.getLineControl());
}

void Sequence::operator+=(const Sequence& seq2)
{
	v.insert(v.end(), seq2.v.begin(), seq2.v.end());
}

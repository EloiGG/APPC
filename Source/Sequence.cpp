/*
  ==============================================================================

	Sequence.cpp
	Created: 23 Jun 2021 11:02:25am
	Author:  admin

  ==============================================================================
*/

#include "Sequence.h"

Sequence Sequence::getNewSequence(const unsigned int& numPrices, const unsigned int& numDigits, Price* prices, const unsigned int& delay_in_milliseconds, const bool& lineControl)
{
	Sequence s;
	s.createSequence(numPrices, numDigits, prices, delay_in_milliseconds, lineControl);
	return s;
}

Sequence::Sequence() :  delay_ms(50)
{
	v.reserve(16);
}

Sequence::Sequence(int reserve) : Sequence()
{
	v.reserve(reserve);
}

void Sequence::addStep(const SequenceStep& newStep)
{
	v.push_back(newStep);
}

void Sequence::createSequence(const unsigned int& numPrices, const unsigned int& numDigits,
	Price* prices, const unsigned int& delay_in_milliseconds, const bool& lineControl)
{
	v.resize(0);
	unsigned char c = lineControl ? 0x46 : 0x41;
	for (int prix = 0; prix < numPrices; prix++) {
		for (int chiffre = 0; chiffre < numDigits; chiffre++) {
			v.push_back({ unsigned char(0x30 + prix * numDigits + chiffre) , c, prices[prix].getUARTchar(chiffre)});
		}
	}
	delay_ms = delay_in_milliseconds;
}


//void Sequence::createSequence(const Core& core)
//{
//	auto& c = core.get();
//	Price prices[Core::MAX_PRICES];
//	for (int i = 0; i < c.getNumPrices(); i++)
//		prices[i] = c.getPrice(i);
//	createSequence(c.getNumPrices(), c.getNumDigits(), prices, c.getDelay(), c.getLineControl());
//}

void Sequence::operator+=(const Sequence& seq2)
{
	v.insert(v.end(), seq2.v.begin(), seq2.v.end());
	delay_ms = jmax(delay_ms, seq2.delay_ms);
}

size_t Sequence::getSize() const
{
	return v.size();
}

Sequence::SequenceStep Sequence::operator[](size_t index) const
{
	return v[index];
}

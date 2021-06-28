/*
  ==============================================================================

	Sequence.h
	Created: 23 Jun 2021 11:02:25am
	Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#pragma once
#include <vector>
#include "Price.h"

class Sequence
{
public:
	struct SequenceStep
	{
		unsigned char adress;
		unsigned char order;
		unsigned char character;
		SequenceStep(const unsigned char& a = 0, const unsigned char& o = 0, const unsigned char& c = 0) : 
			adress(a), order(o), character(c) {}
	};
	static Sequence getNewSequence(const unsigned int& numPrices, const unsigned int& numDigits,
		Price* prices, const unsigned int& delay_in_milliseconds, const bool& lineControl);
	Sequence();
	void addStep(const SequenceStep& newStep);
	void createSequence(const unsigned int& numPrices, const unsigned int& numDigits, 
		Price* prices, const unsigned int& delay_in_milliseconds, const bool& lineControl);
	//void createSequence(const Core& core);
	void operator+=(const Sequence& seq2);
	size_t getSize() const;
	SequenceStep operator[](size_t index) const;
	unsigned int getDelay() const { return delay_ms; }
	

private:
	unsigned int delay_ms;
	std::vector<SequenceStep> v;
};
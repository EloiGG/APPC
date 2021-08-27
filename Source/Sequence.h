/*
  ==============================================================================

	Sequence.h
	Created: 23 Jun 2021 11:02:25am
	Author:  Eloi GUIHARD-GOUJON

	Contient la classe séquence qui englobe une suite d'ordre à transmettre aux modules 7 segments

  ==============================================================================
*/

#pragma once
#include <vector>
#include "Price.h"

/// <summary>
/// Contient une suite d'ordre à envoyer aux modules 7 segments.
/// Chaque ordre est englobé dans la struct SequenceStep.
/// </summary>
class Sequence
{
public:
	// 1 ordre d'une séquence
	struct SequenceStep
	{
		unsigned char adress;
		unsigned char order;
		unsigned char character;
		SequenceStep(const unsigned char& a = 0, const unsigned char& o = 0, const unsigned char& c = 0) : 
			adress(a), order(o), character(c) {}
	};

	Sequence();
	Sequence(int reserve);

	// Crée une séquence à partir des arguments fournis
	void createSequence(const unsigned int& numPrices, const unsigned int& numDigits, 
		Price* prices, const unsigned int& delay_in_milliseconds, const bool& lineControl);

	// Version statique de createSequenc()
	static Sequence getNewSequence(const unsigned int& numPrices, const unsigned int& numDigits,
		Price* prices, const unsigned int& delay_in_milliseconds, const bool& lineControl);

	// Ajoute un ordre à la fin de la séquence
	void addStep(const SequenceStep& newStep);

	// Ajoute la 2e séquence à la suite de la première
	void operator+=(const Sequence& seq2);

	// Retourne la taille de la séquenc
	size_t getSize() const;

	// Retourne le n-ième élément de la séquence
	SequenceStep operator[](size_t index) const;

	// Retourne le délai entre chaque ordre de la séquence
	unsigned int getDelay() const { return delay_ms; }

	void setDelay(unsigned int newDelay) { delay_ms = newDelay; }
	

private:
	unsigned int delay_ms;
	std::vector<SequenceStep> v;
};
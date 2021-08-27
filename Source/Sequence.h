/*
  ==============================================================================

	Sequence.h
	Created: 23 Jun 2021 11:02:25am
	Author:  Eloi GUIHARD-GOUJON

	Contient la classe s�quence qui englobe une suite d'ordre � transmettre aux modules 7 segments

  ==============================================================================
*/

#pragma once
#include <vector>
#include "Price.h"

/// <summary>
/// Contient une suite d'ordre � envoyer aux modules 7 segments.
/// Chaque ordre est englob� dans la struct SequenceStep.
/// </summary>
class Sequence
{
public:
	// 1 ordre d'une s�quence
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

	// Cr�e une s�quence � partir des arguments fournis
	void createSequence(const unsigned int& numPrices, const unsigned int& numDigits, 
		Price* prices, const unsigned int& delay_in_milliseconds, const bool& lineControl);

	// Version statique de createSequenc()
	static Sequence getNewSequence(const unsigned int& numPrices, const unsigned int& numDigits,
		Price* prices, const unsigned int& delay_in_milliseconds, const bool& lineControl);

	// Ajoute un ordre � la fin de la s�quence
	void addStep(const SequenceStep& newStep);

	// Ajoute la 2e s�quence � la suite de la premi�re
	void operator+=(const Sequence& seq2);

	// Retourne la taille de la s�quenc
	size_t getSize() const;

	// Retourne le n-i�me �l�ment de la s�quence
	SequenceStep operator[](size_t index) const;

	// Retourne le d�lai entre chaque ordre de la s�quence
	unsigned int getDelay() const { return delay_ms; }

	void setDelay(unsigned int newDelay) { delay_ms = newDelay; }
	

private:
	unsigned int delay_ms;
	std::vector<SequenceStep> v;
};
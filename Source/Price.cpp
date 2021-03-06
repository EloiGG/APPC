/*
  ==============================================================================

	Price.cpp
	Created: 17 Jun 2021 2:55:28pm
	Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#include "Price.h"

Price::Price(const String& price)
{
	if (isValid(price))
		*this = price;
}

void Price::operator=(const String& newPrice)
{
	auto s = newPrice;
	if (s.length() >= MAX_LENGH) {
		s = s.substring(0, MAX_LENGH);
	}
	if (s.length() != 0)
	{
		if (s[1] == '.')
			s = s.upToFirstOccurrenceOf(".", false, false) + s.substring(s.indexOfChar('.') + 1);
		else if (s[1] == ',')
			s = s.upToFirstOccurrenceOf(",", false, false) + s.substring(s.indexOfChar(',') + 1);
		if (isValid(s)) {
			priceString = s;
			for (int i = s.length() - 1; i < MAX_LENGH; i++)
				priceString += '0';
		}
		else
			DBG("Chaine invalide : " << s);
	}
}

String Price::operator[](int index) const
{
	return priceString.substring(index, index + 1);
}

unsigned char Price::toUARTchar(const char c)
{
	unsigned char defaultAnswer = 0x0a; // mise au noir

	if (c >= 0x30 && c <= 0x39)
		return c;
	else if (c >= 0x41 && c <= 0x5a) {
		if (c == 0x57 || c == 0x56)
			return defaultAnswer;
		else
			return c;
	}
	else if (c >= 0x61 && c <= 0x7a) {
		if (c == 0x6f)
			return 0x2e;
		else if (c == 0x77)
			return defaultAnswer;
		else
			return c;
	}
	else if (c == ' ')
		return 0x0a;
	else if (c == '-')
		return 0x2d;
	else {
		DBG("Caractere impossible a envoyer" << c);
		return defaultAnswer;
	}
}

bool Price::isValid(const String& s)
{
	if (s.length() == 0)
		return false;	
	for (int i = 0; i < s.length(); i++)
	{
		auto c = s[i];
		if (i == 1 && c == '.') continue;
		if (c >= 0x30 && c <= 0x39) continue;
		else if (c >= 0x41 && c <= 0x5a) {
			if (c == 0x57 || c == 0x56) return false;
			continue;
		}
		else if (c >= 0x61 && c <= 0x7a) {
			if (c == 0x77) return false;
			continue;
		}
		else if (c == '-' || c == ' ') continue;
		else return false;
	}
	return true;
}


String Price::toString(size_t stringLengh, bool withComa) const
{
	if (!withComa) return priceString;
	String s(priceString.substring(0, 1));
	s.append(".", 1);
	s.append(priceString.substring(1, stringLengh), stringLengh);
	return s;
}


void Price::changeOneDigit(unsigned int digitIndex, const String& newDigit)
{
	if (Price::isValid(newDigit))
	{
		auto s = priceString.substring(0, digitIndex);
		s.append(newDigit, 1);
		s.append(priceString.substring(digitIndex + 1), MAX_LENGH);
		priceString = s;
	}
}

bool Price::isEmpty()
{
	return priceString.length() == 0;
}

unsigned char Price::getUARTchar(int index)
{
	return toUARTchar(priceString[index]);
}

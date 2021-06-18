/*
  ==============================================================================

	Price.cpp
	Created: 17 Jun 2021 2:55:28pm
	Author:  admin

  ==============================================================================
*/

#include "Price.h"

Price::Price(const String& price)
{
	*this = price;
}

void Price::operator=(const String& newPrice)
{
	auto s = newPrice;
	if (s.length() >= MAX_LENGH) {
		s = s.substring(0, MAX_LENGH);
	}
	else if (s.length() != 0)
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

String Price::operator[](int index)
{
	return priceString.substring(index, index+1);
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
	bool r = true;
	for (char c : s)
	{
		if (c >= 0x30 && c <= 0x39);
		else if (c >= 0x41 && c <= 0x5a) {
			if (c == 0x57 || c == 0x56)
				return false;
		}
		else if (c >= 0x61 && c <= 0x7a) {
			if (c == 0x77)
				return false;
		}
		else if (c == '-')
			return c;
		else
			return false;
	}
	return true;
}

void Price::convert(String& s)
{
	char c[MAX_LENGH + 1];

	s = c;
}

String Price::toString(size_t stringLengh)
{
	return String(priceString[0] + "." + priceString.substring(2, stringLengh - 2));
}

//
//void Price::changeOneDigit(unsigned int digitIndex, String& newDigit)
//{
//	priceString.replaceSection(digitIndex, digitIndex, newDigit);
//
//}

/*
  ==============================================================================

    Price.h
    Created: 17 Jun 2021 2:55:28pm
    Author:  admin

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class Price
{
public:
    static constexpr size_t MAX_LENGH = 10; // pas plus de 6 chiffres par prix
    Price(const String& price = "");
    void operator=(const String& newPrice);
    String operator[](int index) const;
    static unsigned char toUARTchar(const char c);
    static bool isValid(const String& s);
    static void convert(String& s);
    String toString(size_t stringLengh) const;
    void changeOneDigit(unsigned int digitIndex, const String& newDigit);
    String priceString;
    bool isEmpty();

private:
};
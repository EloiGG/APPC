/*
  ==============================================================================

    Price.h
    Created: 17 Jun 2021 2:55:28pm
    Author:  admin

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <winnt.h>

class Price
{
    static constexpr size_t MAX_LENGH = 6; // pas plus de 6 chiffres par prix
public:
    Price(const String& price = "");
    void operator=(const String& newPrice);
    String operator[](int index);
    static unsigned char toUARTchar(const char c);
    static bool isValid(const String& s);
    static void convert(String& s);
    String toString(size_t stringLengh);
    //void changeOneDigit(unsigned int digitIndex, String& newDigit);

private:
    String priceString;
};
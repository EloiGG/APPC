/*
  ==============================================================================

    Price.h
    Created: 17 Jun 2021 2:55:28pm
    Author:  Eloi GUIHARD-GOUJON

    Contient la classe Price qui symbolise un prix

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

/// <summary>
/// Classe symbolisant un prix
/// </summary>
class Price
{
public:
    static constexpr size_t MAX_LENGH = 10; // pas plus de 10 chiffres par prix
    
    Price(const String& price = "");

    // Permet de créer un prix grâce à une chaine de caractères (avec ou sans virgule)
    void operator=(const String& newPrice);

    // Accède au n-ième caractère du prix
    String operator[](int index) const;

    // Convertit un caractère en caractère affichable par un module 7 segments
    static unsigned char toUARTchar(const char c);

    // Retourne vrai si la chaîne de caractère représente un prix valide
    static bool isValid(const String& s);

    // Convertit le prix en chaîne de caractères
    String toString(size_t stringLengh, bool withComa = true) const;

    // Change un caractère du prix
    void changeOneDigit(unsigned int digitIndex, const String& newDigit);

    // Retourne vrai si le prix ext vide 
    bool isEmpty();

    // Traduit le n-ième caractère du prix en caractère affichable par un module 7 segments
    unsigned char getUARTchar(int index);

private:
    String priceString;
};
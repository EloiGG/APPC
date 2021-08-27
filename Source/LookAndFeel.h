/*
  ==============================================================================

    LookAndFeel.h
    Created: 17 Jun 2021 11:04:18am
    Author:  Eloi GUIHARD-GOUJON

    Contient les éléments spécifiant le rendu visuel du logiciel

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

// Couleurs communes dans le logiciel
namespace lfColours
{
    const Colour digitColour(244, 255, 143);
    const Colour priceBackground(22, 22, 22);
    const Colour digitBackground(0, 0, 0);
    const Colour onDigitUpdate(255, 53, 46);
    const Colour priceDisplayBackground(43, 47, 48);
    const Colour panelBackground(50, 50, 50);
    const Colour tabBackground(31, 30, 28);
    const Colour buttonBackground(60, 67, 79);
    const Colour inputOutline(204, 204, 204);
    const Colour inputBackground(tabBackground.darker());
    const Colour sendButton(77, 140, 67);
    const Colour stopButton(255, 46, 46);
    const Colour errModuleOK(50, 201, 44);
    const Colour errModuleWIP(255, 255, 255 );
    const Colour errModuleSegment(227, 121, 39);
    const Colour errModuleTimeout(255, 0, 0);
    const Colour keyboardBack(204, 164, 71);
    const Colour keyboardCancel(166, 30, 30);
    const Colour keyboardConfirm(66, 171, 56);
    const Colour upToDate(186, 117, 255);
}

/// <summary>
/// Classe permettant de changer les visuels des éléments du logiciel
/// </summary>
class APPCLookAndFeel : public LookAndFeel_V4
{
public:
    APPCLookAndFeel() : font("Agency BF", "Bold", 14) {}

    virtual void drawButtonBackground(Graphics&, Button&, const Colour& backgroundColour,
        bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

    Font getTextButtonFont(TextButton&, int buttonHeight) override;

    void drawTickBox(Graphics& g, Component& component,
        float x, float y, float w, float h,
        const bool ticked,
        const bool isEnabled,
        const bool shouldDrawButtonAsHighlighted,
        const bool shouldDrawButtonAsDown) override;

    void drawToggleButton(Graphics& g, ToggleButton& button,
        bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

private:
    Font font;
};

class KeyboardLookAndFeel : public APPCLookAndFeel
{
public:
    Font getTextButtonFont(TextButton&, int buttonHeight) override;
};
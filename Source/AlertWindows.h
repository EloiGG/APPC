/*
  ==============================================================================

    AlertWindows.h
    Created: 23 Jul 2021 1:41:23pm
    Author:  Eloi GUIHARD-GOUJON

    Permet d'afficher les fen�tres d'alerte

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

/// <summary>
/// Classe permettant d'afficher diff�rentes fen�tres d'alerte
/// </summary>
class APPCAlertWindows
{
public:
    enum class WindowType{
        PriceChoice, LoadFromCentoFuel, NoConnection
    };

public:
    APPCAlertWindows();
    ~APPCAlertWindows() { }

    // Ouvre la fen�tre d'alerte d�sir�e
    void open(WindowType window, const String& message = "", const std::function<void(int)>& callbackfunction = std::function<void(int)>());

    // Ferme toutes les fen�tres d'alerte ouverte
    void close();
private:
    AlertWindow* aw;
};
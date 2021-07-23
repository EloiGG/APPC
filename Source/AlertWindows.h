/*
  ==============================================================================

    AlertWindows.h
    Created: 23 Jul 2021 1:41:23pm
    Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class APPCAlertWindows
{
public:
    enum class WindowType{
        PriceChoice, LoadFromCentoFuel
    };

public:
    APPCAlertWindows();
    ~APPCAlertWindows() { }

    void open(WindowType window, const std::function<void(int)>& callbackfunction = std::function<void(int)>());

private:
    AlertWindow* aw;
};
/*
  ==============================================================================

    Networking.h
    Created: 16 Jun 2021 10:59:25am
    Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/
#pragma once
#include <JuceHeader.h>
#include <tuple>

class Network 
{
public:
    static bool err_ok(const String& output);

public:
    using String = juce::String;

    Network();
    Network(const String& token, const String& password);
    String getFuelPrice(int timeout_ms = 10000);
    void setAuthentication(const String& token, const String& password);
private:
    String makeHeader();
    String authToken, authPassword;
    String url;
};

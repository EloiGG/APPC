/*
  ==============================================================================

    SerialThread.h
    Created: 23 Jun 2021 10:29:11am
    Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "UART.h"
class SerialThread : public Thread
{
public:
    SerialThread();
private:
    virtual void run() override;
};
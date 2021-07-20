/*
  ==============================================================================

    COMThread.h
    Created: 20 Jul 2021 8:10:41am
    Author:  Eloi GUIHARD-GOUJON    

  ==============================================================================
*/
#pragma once
#include "Buttons.h"
#include <vector>
class COMThread : public ProgressionThread
{
public:
    virtual void run() override;
    bool verifyCOMPort(unsigned int portNum);
    bool getModuleResponse(int digitNumber);
private:
    Stopwatch s;
    UART uart;
};

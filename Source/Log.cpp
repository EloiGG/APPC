/*
  ==============================================================================

    Log.cpp
    Created: 28 Jun 2021 8:56:29am
    Author:  admin

  ==============================================================================
*/

#include "Log.h"

Log Log::_log;

void Log::write(const String& string, int debugLevel)
{
    _log.writeImpl(string, debugLevel);
}

void Log::writeImpl(const String& string, int debugLevel)
{
    for (int i = 0; i < debugLevel; i++)
        s[i].append(string, string.length());
}

void Log::update()
{
    _log.updateImpl();
}

void Log::updateImpl()
{
    updateFunction();
}

Log::Log()
{
}

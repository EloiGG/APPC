/*
  ==============================================================================

    GridTool.h
    Created: 17 Jun 2021 9:23:59am
    Author:  Eloi GUIHARD-G0UJON

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class GridTool  : public juce::Component
{
public:
    GridTool(int columns = 0, int rows = 0) : numcolumns(columns), numrows(rows)
    {
    }

    ~GridTool() override
    {
    }

    void resize(int columns, int rows)
    {
        numcolumns = columns;
        numrows = rows;
    }

    bool isSet()
    {
        return numcolumns != 0 && numrows != 0;
    }

    void paint (juce::Graphics& g) override
    {
        g.setColour(Colours::black);
        g.setOpacity(0.5f);
        for (int r = 0; r < numrows; r++)
            g.drawLine(0, jmap(r, 0, numrows , 0, getHeight()),
                getWidth(), jmap(r, 0, numrows , 0, getHeight()));
        for (int c = 0; c < numcolumns; c++)
            g.drawLine(jmap(c, 0, numcolumns , 0, getWidth()), 0,
                jmap(c, 0, numcolumns , 0, getWidth()), getHeight());
    }

    Point<int> getPoint(float x, float y)
    {
        return Point<int>{ int(getWidth() * x / numcolumns), int(getHeight() * y / numrows) };
    }

    Rectangle<int> getRectangle(float x1, float y1, float x2, float y2)
    {
        return { getPoint(x1,y1), getPoint(x2,y2) };
    }

    void resized() override
    {
    }

    int getNumRows()
    {
        return numrows;
    }

    int getNumColumns()
    {
        return numcolumns;
    }

private:
    int numcolumns, numrows;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GridTool)
};
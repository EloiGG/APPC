/*
  ==============================================================================

	DialogBoxComponent.h
	Created: 20 Jul 2021 4:21:58pm
	Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Disabled.h"
#include "LookAndFeel.h"
//==============================================================================
/*
*/
class DialogBoxComponent : public juce::Component
{
public:
	DialogBoxComponent(Component* componentToPlaceInside) : insideComponent(componentToPlaceInside), w(0), h(0)
	{
		setInterceptsMouseClicks(false, false);
		setAlwaysOnTop(true);
		addAndMakeVisible(background);
		addAndMakeVisible(*insideComponent);
	}

	~DialogBoxComponent() override {}

	void resized() override
	{
		background.setBounds(getLocalBounds());
		insideComponent->centreWithSize(w * getWidth(), h * getHeight());
	}

	void open()
	{
		setInterceptsMouseClicks(true, true);
		background.setDisabled(true);
		insideComponent->setVisible(true);
		onOpenning();
	}

	void close()
	{
		setInterceptsMouseClicks(false, false);
		background.setDisabled(false);
		insideComponent->setVisible(false);
	}

protected:
	virtual void onOpenning() {}
	void setInteriorProportions(float widthProportion, float heightProportion)
	{
		w = widthProportion; h = heightProportion;
	}
	std::unique_ptr<Component> insideComponent;

private:
	float w, h;
	Disabled background;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DialogBoxComponent)
};

class CustomTableListBox : public TableListBoxModel, public Component
{
public:
	CustomTableListBox(size_t numRows = 0) : table{ {},this }, font(14.0f), size(numRows)
	{
		addAndMakeVisible(table); 
		table.setColour(TableListBox::ColourIds::backgroundColourId, Colours::black);
		table.setColour(juce::ListBox::outlineColourId, juce::Colours::grey);
		table.autoSizeAllColumns();
	}
	int getNumRows() override { return size; }
	void paintRowBackground(Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) override 
	{
		Colour c(lfColours::panelBackground);
		if (rowNumber % 2)
			c = c.darker();
		g.fillAll(c);
	}
	void paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override
	{
		g.setFont(font);
		g.setColour(Colours::white);
		String text = getPropriety(rowNumber, columnId);
		g.drawText(text, 2, 0, width, height, Justification::centredLeft);
	}
	virtual void cellClicked(int rowNumber, int columnId, const MouseEvent&) = 0;
	int getColumnAutoSizeWidth(int columnId) override
	{
		int m = 0;
		for (int i = 0; i < size; ++i) {
			String text = getPropriety(i, columnId);
			m = jmax(m, font.getStringWidth(text));
		}
		m = jmax(m, font.getStringWidth(table.getHeader().getColumnName(columnId)));
		return m + 10;
	}
	virtual String getPropriety(int row, int column) = 0;

	void paint(juce::Graphics& g)
	{
		g.fillAll(lfColours::panelBackground);
	}

	void resized()
	{
		table.setBoundsInset(juce::BorderSize<int>(8));
	}

	
	void setMousePosition(const Point<int> newPosition) { mousePosition = newPosition; }
protected:
	juce::TableListBox table;
	size_t size;
private:

	Font font;
	Point<int> mousePosition;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomTableListBox)
};
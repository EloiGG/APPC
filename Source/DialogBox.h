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

class CustomTableListBox : public TableListBoxModel, public Component
{
public:
	CustomTableListBox(size_t numRows = 0) : table{ {},this }, font(14.0f), size(numRows), desiredWidth(0)
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

	int getVerticalScrollbarWidth()
	{
		return (!table.getVerticalScrollBar().isVisible()) * 8;
	}

	int getHorizontalScrollbarHeight()
	{
		DBG((!table.getHorizontalScrollBar().isVisible()) * 8);
		return (!table.getHorizontalScrollBar().isVisible()) * 8;
	}

	void computeDesiredProportions()
	{
		unsigned int width = 0;
		for (int i = 1; i <= table.getHeader().getNumColumns(true); i++)
			width += getColumnAutoSizeWidth(i);
		desiredWidth = width + 32;
		desiredHeight = size * table.getRowHeight() + table.getHeaderHeight() + 26;
	}

	unsigned int getDesiredWidth() { return desiredWidth; }
	unsigned int getDesiredHeight() { return desiredHeight; }

protected:
	juce::TableListBox table;
	size_t size;

private:
	unsigned int desiredWidth, desiredHeight;
	Font font;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomTableListBox)
};


class DialogBoxComponent : public juce::Component
{
public:
	DialogBoxComponent(CustomTableListBox* componentToPlaceInside) : insideComponent(componentToPlaceInside), w(0), h(0)
	{
		setInterceptsMouseClicks(false, false);
		setAlwaysOnTop(true);
		addAndMakeVisible(background);
		addAndMakeVisible(*insideComponent);

		close();
	}

	~DialogBoxComponent() override {}

	void resized() override
	{
		background.setBounds(getLocalBounds());
		int dw = insideComponent->getDesiredWidth() != 0 ? insideComponent->getDesiredWidth() : w * getWidth(),
			dh = insideComponent->getDesiredHeight() != 0 ? insideComponent->getDesiredHeight() : h * getHeight();
		insideComponent->centreWithSize(jmin<int>(w * getWidth(), dw), jmin<int>(h * getHeight(), dh));
	}

	void open()
	{
		setVisible(true);
		setInterceptsMouseClicks(true, true);
		background.setDisabled(true);
		insideComponent->setVisible(true);
		repaint();
	}

	void close()
	{
		setVisible(false);
		setInterceptsMouseClicks(false, false);
		background.setDisabled(false);
		insideComponent->setVisible(false);
		repaint();

	}

protected:
	virtual void onOpenning() {}
	void setInteriorProportions(float widthProportion, float heightProportion)
	{
		w = widthProportion; h = heightProportion;
	}
	std::unique_ptr<CustomTableListBox> insideComponent;

private:
	float w, h;
	Disabled background;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DialogBoxComponent)
};
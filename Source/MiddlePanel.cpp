/*
  ==============================================================================

	MiddlePanel.cpp
	Created: 19 Jun 2021 1:07:43am
	Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#include "MiddlePanel.h"

//==============================================================================
MiddlePanel::MiddlePanel() : topGrid(5, 1), leftGrid(1, 5)
{
	addAndMakeVisible(prices);
	addAndMakeVisible(cornerDigit);
	cornerDigit.setShowState(false);

	// colonnes
	for (int i = 0; i < Core::MAX_DIGITS; i++) {
		topDigits[i].setShowState(false);
		topDigits[i].textManuallyUpdated = [this, i]()
		{
			for (int j = 0; j < Core::get().getNumPrices(); ++j) {
				auto p = Core::get().getPrice(j);
				p.changeOneDigit(i, topDigits[i].getText());
				Core::get().setPrice(j, p);
			}
			topDigits[i].setText(" ", NotificationType::sendNotification);
			for (int j = 0; j < Core::get().getNumPrices(); ++j)
				updatePrices(TextUpdateOrigin::Omni, j);
			updateVisualization();
		};
	}
	for (int i = 0; i < Core::get().getNumDigits(); i++)
		addAndMakeVisible(topDigits[i]);
	// lignes
	for (int i = 0; i < Core::MAX_PRICES; i++) {
		leftDigits[i].setShowState(false);
		leftDigits[i].textManuallyUpdated = [this, i]()
		{
			char s[Core::MAX_DIGITS];
			for (int j = 0; j < Core::MAX_DIGITS; ++j)
				s[j] = leftDigits[i].getText().getCharPointer()[0];
			Core::get().setPrice(i, Price(s));
			leftDigits[i].setText(" ", NotificationType::sendNotification);
			updatePrices(TextUpdateOrigin::Omni, i);
			updateVisualization();
		};
	}
	for (int i = 0; i < Core::get().getNumPrices(); i++)
		addAndMakeVisible(leftDigits[i]);
	// tout
	cornerDigit.textManuallyUpdated = [this]()
	{
		char s[Core::MAX_DIGITS];
		for (int j = 0; j < Core::MAX_DIGITS; ++j)
			s[j] = cornerDigit.getText().getCharPointer()[0];
		for (int j = 0; j < Core::get().getNumPrices(); j++) {
			Core::get().setPrice(j, Price(s));
			updatePrices(TextUpdateOrigin::Omni, j);
		}
		cornerDigit.setText(" ", NotificationType::sendNotification);
		updateVisualization();
	};
	addAndMakeVisible(highlights);
	highlights.resize(4, 4);
	addAndMakeVisible(topGrid);
	addAndMakeVisible(leftGrid);
}

MiddlePanel::~MiddlePanel()
{
}

void MiddlePanel::paint(juce::Graphics& g)
{
	g.fillAll(lfColours::panelBackground);
	auto heightdiff = topGrid.getHeight() - topDigits[0].getHeight();
	auto trianglewidth = heightdiff / 4;
	Path p;
	bool mouseOnDigit = false;
	for (size_t i = 0; i < Core::get().getNumDigits(); i++)
	{
		// dessin triangle
		Point<float> startPoint({ (float)topDigits[i].getBounds().getCentreX(),
			(float)topDigits[i].getBottom() + heightdiff * 0.5f - trianglewidth / 2 });
		p.startNewSubPath(startPoint);
		p.lineTo(startPoint.translated(trianglewidth, -trianglewidth));
		p.lineTo(startPoint.translated(-trianglewidth, -trianglewidth));
		p.closeSubPath();

		// surligner
		if (topDigits[i].isMouseOver(true)) {
			highlights.highlightColumn(i);
			mouseOnDigit = true;
		}
	}
	auto widthdiff = leftGrid.getWidth() - leftDigits[0].getWidth();
	trianglewidth = jmin(trianglewidth, widthdiff / 4);
	for (size_t i = 0; i < Core::get().getNumPrices(); i++)
	{
		// dessin triangle
		Point<float> startPoint({ (float)leftDigits[i].getRight() + widthdiff * 0.25f + trianglewidth / 2,
			(float)leftDigits[i].getBounds().getCentreY() });
		p.startNewSubPath(startPoint);
		p.lineTo(startPoint.translated(-trianglewidth, -trianglewidth));
		p.lineTo(startPoint.translated(-trianglewidth, trianglewidth));
		p.closeSubPath();

		// surligner
		if (leftDigits[i].isMouseOver(true)) {
			highlights.highlightRow(i);
			mouseOnDigit = true;
		}
	}
	Point<float> startPoint(topGrid.getPoint(0, 1, true).toFloat());
	auto translation = jmin(widthdiff, heightdiff) * 0.25;
	startPoint.applyTransform(AffineTransform::translation(-translation, -translation));
	p.startNewSubPath(startPoint);
	p.lineTo(startPoint.translated(0, -trianglewidth * 1.4f));
	p.lineTo(startPoint.translated(-trianglewidth * 1.4f, 0));
	p.closeSubPath();
	if (cornerDigit.isMouseOver(true)) {
		highlights.highlightAll();
		mouseOnDigit = true;
	}

	if (!mouseOnDigit)
		highlights.stopHighlighting();

	g.setColour(Colours::red);
	g.fillPath(p);
}

void MiddlePanel::resized()
{
	auto b = prices.getFittingRectangle(getLocalBounds());
	int margin = jmin(getWidth(), getHeight()) * 0.2;

	auto leftRect = b.removeFromLeft(margin);
	auto cornerRect = leftRect.removeFromTop(margin);

	b = prices.getFittingRectangle(getLocalBounds());
	auto topRect = b.removeFromTop(margin);
	topRect.removeFromLeft(margin);
	b.removeFromLeft(margin);

	topGrid.setBounds(topRect);
	leftGrid.setBounds(leftRect);

	highlights.setBounds(b);
	float whRatio = 4.0f / 5.0f;
	float smaller = 0.1f;
	for (int i = 0; i < Core::get().getNumDigits(); i++) {
		auto rect = topGrid.getRectangle(i, 0, i + 1, 1, true);
		auto rect2 = rect.withWidth(rect.getHeight() * whRatio).withCentre(rect.getCentre()).reduced(rect.getWidth() * smaller);
		topDigits[i].setBounds(rect2);
	}
	for (int i = 0; i < Core::get().getNumPrices(); i++) {
		auto rect = leftGrid.getRectangle(0, i, 1, i + 1, true);
		auto rect2 = rect.withWidth(rect.getHeight() * whRatio).withCentre(rect.getCentre()).reduced(rect.getWidth() * smaller);
		leftDigits[i].setBounds(rect2);
	}
	cornerDigit.setBounds(
		cornerRect.withWidth(cornerRect.getWidth() * whRatio).withCentre(cornerRect.getCentre()).reduced(cornerRect.getWidth() * smaller));
	prices.setBounds(b);
}

void MiddlePanel::updatePrices(TextUpdateOrigin whocalled, unsigned int index)
{
	prices.updatePrices(whocalled, index);
}

void MiddlePanel::updateVisualization()
{
	auto ndigits = Core::get().getNumDigits(), nprices = Core::get().getNumPrices();
	int i;
	for (i = 0; i < ndigits; i++)
		addAndMakeVisible(topDigits[i]);
	for (; i < Core::MAX_DIGITS; i++)
		topDigits[i].setVisible(false);
	for (i = 0; i < nprices; i++)
		addAndMakeVisible(leftDigits[Core::MAX_DIGITS + i]);
	for (; i < Core::MAX_DIGITS; ++i)
		leftDigits[i].setVisible(false);

	topGrid.resize(ndigits, 1);
	leftGrid.resize(1, nprices);
	prices.setNumPrices(nprices);
	prices.setNumDigits(ndigits);
}

void MiddlePanel::mouseExit(const MouseEvent&)
{
	repaint();
}

void MiddlePanel::mouseEnter(const MouseEvent&)
{
	repaint();
}

HighLight::HighLight()
{
	setOpaque(false);
	setInterceptsMouseClicks(false, false);
}

void HighLight::resize(unsigned int numColumns, unsigned int numLines)
{
	grid.resize(numColumns, numLines);
}

void HighLight::highlightRow(int rowIndex)
{
	hlRect = grid.getRectangle(0, rowIndex, grid.getNumColumns(), rowIndex + 1, true);
	repaint();
}

void HighLight::highlightColumn(int columnIndex)
{
	hlRect = grid.getRectangle(columnIndex, 0, columnIndex + 1, grid.getNumRows(), true);
	repaint();
}

void HighLight::highlightAll()
{
	hlRect = getLocalBounds();
	repaint();
}

void HighLight::stopHighlighting()
{
	hlRect = juce::Rectangle<int>(0, 0, 0, 0);
}

void HighLight::paint(juce::Graphics& g)
{
	g.setColour(Colours::white.withAlpha(0.2f));
	g.fillRect(hlRect);
}

void HighLight::resized()
{
	grid.setBounds(getLocalBounds());
}

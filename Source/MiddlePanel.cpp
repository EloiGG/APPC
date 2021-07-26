/*
  ==============================================================================

	MiddlePanel.cpp
	Created: 19 Jun 2021 1:07:43am
	Author:  Eloi GUIHARD-GOUJON

  ==============================================================================
*/

#include "MiddlePanel.h"

//==============================================================================

MiddlePanel::MiddlePanel() : topGrid(5, 1), leftGrid(1, 5), name("", L"Nom de l'UC"),
openConfig("openconfig", Colours::grey, Colours::grey.brighter(), Colours::grey.brighter()),
nDigits(0), nPrices(0)
{
	addAndMakeVisible(disable);
	addAndMakeVisible(buttons);
	addAndMakeVisible(prices);
	addAndMakeVisible(cornerDigit);
	addAndMakeVisible(name);
	addAndMakeVisible(openConfig);
	name.setJustificationType(Justification::centred);
	cornerDigit.setShowState(false);

	disable.setDisabled(false);
	Core::get().showKeyboard = [this](KeyboardLabel* caller, const String& startingText = "", unsigned int maxChar = Core::MAX_DIGITS)
	{
		kb.resetAndShow(caller, startingText, maxChar);
		Timer::callAfterDelay(15, [this]() {kb.grabKeyboardFocus(); });;
	};
	Path p;
	int i;
	for (i = 0; i < 7; i++) {
		p.addLineSegment(Line<float>(3, 4 * i, 7, 4 * i), 1.0);
		p.addLineSegment(Line<float>(10, 4 * i, 20, 4 * i), 1.0);
	}
	p.addLineSegment(Line<float>(0, -4, 23, -4), 1.0);
	p.addLineSegment(Line<float>(23, -4, 23, 4 * (i + 0.5f)), 1.0);
	p.addLineSegment(Line<float>(23, 4 * (i + 0.5f), 0, 4 * (i + 0.5f)), 1.0);
	p.addLineSegment(Line<float>(0, 4 * (i + 0.5f), 0, -4), 1.0);
	openConfig.setShape(p, false, true, true);
	openConfig.setClickingTogglesState(true);
	openConfig.shouldUseOnColours(true);
	openConfig.setOnColours(Colours::white, Colours::white, Colours::white);


	// colonnes
	for (int i = 0; i < Core::MAX_DIGITS; i++) {
		topDigits[i].setShowState(false);
		topDigits[i].onTextKeyboardUpdate = [this, i]()
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
		leftDigits[i].onTextKeyboardUpdate = [this, i]()
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
	cornerDigit.onTextKeyboardUpdate = [this]()
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
	highlights.resize(Core::get().getNumDigits(), Core::get().getNumPrices());

	openConfig.onClick = [this]()
	{
		if (openConfig.getToggleState())
			Core::get().openSettings();
		else
			Core::get().closeSettings();
	};

	kb.setAlwaysOnTop(true);
	disable.setAlwaysOnTop(true);
	addChildComponent(kb);
}

MiddlePanel::~MiddlePanel()
{
}

void MiddlePanel::paint(juce::Graphics& g)
{
	g.fillAll(lfColours::panelBackground);
	g.setColour(lfColours::priceBackground);
	g.fillRect(nameArea);
	g.setColour(Colours::black);
	g.drawRect(nameArea);
	g.fillRect(openConfigArea);
	g.setColour(openConfig.getToggleState() ? Colours::white : Colours::grey);
	g.drawRect(openConfigArea);
	if (Core::get().getNumDigits() == 1 || Core::get().getNumPrices() == 1) return;
	auto heightdiff = topGrid.getHeight() - topDigits[0].getHeight();
	auto trianglewidth = jmin(heightdiff / 4, topGrid.getRectangle(0, 0, 1, 1).getWidth() / 10);
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
	kb.setBounds(getLocalBounds());

	auto digitsSpace = getLocalBounds();
	auto buttonSpace = digitsSpace.removeFromBottom(digitsSpace.getHeight() * 0.1);
	nameArea = digitsSpace.removeFromTop(digitsSpace.getHeight() * 0.1);

	disable.setBounds(digitsSpace);

	int configButtonSize = nameArea.getHeight();
	auto r = nameArea;
	openConfigArea = r.removeFromRight(configButtonSize);
	openConfig.setBounds(openConfigArea);

	Font f(jmax<int>(nameArea.getHeight() * 0.5, 16));
	name.setFont(f);
	name.setBounds(nameArea);

	buttons.setBounds(buttonSpace);
	auto b = prices.getFittingRectangle(digitsSpace);

	if (Core::get().getNumDigits() != 1 && Core::get().getNumPrices() != 1)
	{
		int marginX = b.getWidth() * 0.2, marginY = b.getHeight() * 0.2;
		marginX = jmin(marginX, marginY);
		marginY = marginX;

		auto leftRect = b.removeFromLeft(marginX);
		auto cornerRect = leftRect.removeFromTop(marginY);

		b = prices.getFittingRectangle(digitsSpace);
		auto topRect = b.removeFromTop(marginY);
		topRect.removeFromLeft(marginX);
		b.removeFromLeft(marginX);

		topGrid.setBounds(topRect);
		leftGrid.setBounds(leftRect);
		highlights.setBounds(b);
		float whRatio = 4.0f / 5.0f;
		float smaller = 0.1f;
		auto rectDimensions = topGrid.getRectangle(0, 0, 1, 1, true).getHeight() > leftGrid.getRectangle(0, 0, 1, 1, true).getHeight() ?
			leftGrid.getRectangle(0, 0, 1, 1, true) : topGrid.getRectangle(0, 0, 1, 1, true);
		int dimX = rectDimensions.getHeight() * whRatio, dimY = rectDimensions.getHeight();
		if (dimX > topGrid.getRectangle(0, 0, 1, 1, true).getWidth()) {
			dimX = topGrid.getRectangle(0, 0, 1, 1, true).getWidth();
			dimY = dimX / whRatio;
		}
		if (dimX > leftGrid.getRectangle(0, 0, 1, 1, true).getWidth()) {
			dimX = leftGrid.getRectangle(0, 0, 1, 1, true).getWidth();
			dimY = dimX / whRatio;
		}
		for (int i = 0; i < Core::get().getNumDigits(); i++) {
			auto rect = topGrid.getRectangle(i, 0, i + 1, 1, true).withSizeKeepingCentre(dimX, dimY);
			auto rect2 = rect.reduced(rect.getWidth() * smaller);
			topDigits[i].setBounds(rect2);
		}
		for (int i = 0; i < Core::get().getNumPrices(); i++) {
			auto rect = leftGrid.getRectangle(0, i, 1, i + 1, true).withSizeKeepingCentre(dimX, dimY);
			auto rect2 = rect.reduced(rect.getWidth() * smaller);
			leftDigits[i].setBounds(rect2);
		}
		auto rect = cornerRect.withSizeKeepingCentre(dimX, dimY);
		auto rect2 = rect.reduced(rect.getWidth() * smaller);
		cornerDigit.setBounds(rect2);
	}

	prices.setBounds(b);
}

void MiddlePanel::updatePrices(TextUpdateOrigin whocalled, unsigned int index)
{
	prices.updatePrices(whocalled, index);
}

void MiddlePanel::updateVisualization()
{
	auto& c = Core::get();
	auto ndigits = c.getNumDigits(), nprices = c.getNumPrices();
	int i;
	cornerDigit.setVisible(true);
	for (i = 0; i < ndigits; i++)
		addAndMakeVisible(topDigits[i]);
	for (; i < Core::MAX_DIGITS; i++)
		topDigits[i].setVisible(false);
	for (i = 0; i < nprices; i++)
		addAndMakeVisible(leftDigits[i]);
	for (; i < Core::MAX_PRICES; ++i)
		leftDigits[i].setVisible(false);
	if (ndigits == 1 || nprices == 1) {
		for (int i = 0; i < c.getNumPrices(); i++)
			leftDigits[i].setVisible(false);
		for (int i = 0; i < c.getNumDigits(); i++)
			topDigits[i].setVisible(false);
		cornerDigit.setVisible(false);
	}
	highlights.resize(ndigits, nprices);
	topGrid.resize(ndigits, 1);
	prices.setNumDigits(ndigits);

	leftGrid.resize(1, nprices);
	prices.setNumPrices(nprices);

	name.setText(c.getUCName(), NotificationType::sendNotification);
	buttons.updateVizualisation();
	if (c.isInSelection())
		disable.setDisabled(false);
	else if (!c.isInit())
		disable.setDisabled(true);
	else
		disable.setDisabled(false);

	repaint();
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

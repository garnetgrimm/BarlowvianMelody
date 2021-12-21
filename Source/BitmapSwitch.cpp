#include "BitmapSwitch.h"
#include "PluginConstants.h"

void BitmapSwitchLookAndFeel::incBitmapFrame() {
	frame = ++frame % (img.getHeight() / animSize);
}

int BitmapSwitchLookAndFeel::getBitmapFrame() {
	return frame;
}

float BitmapSwitchLookAndFeel::getAnimPercent() {
	int maxFrame = (img.getHeight() / animSize) - 1;
	return (float)frame / (float)maxFrame;
}

void BitmapSwitchLookAndFeel::setBounds(int x, int y, int width, int height) {
	sliderBounds.setWidth(width);
	sliderBounds.setHeight(animSize + INFO_PADDING + FONT_HEIGHT);
	textBounds.setBounds(0, FONT_HEIGHT + INFO_PADDING + img.getHeight() + INFO_PADDING, width, TEXTBOX_SIZE);
}

void BitmapSwitchLookAndFeel::setBitmapState(Image img, int animSize, std::string title) {
	this->img = img;
	this->animSize = animSize;
	this->title = title;
}

void BitmapSwitchLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
	const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider) {

	int yMin = min(sliderPos * img.getHeight(), (float)img.getHeight() - animSize);
	int yMax = yMin + animSize;
	int xMin = 0;
	int xMax = animSize;

	g.drawImage(img, (sliderBounds.getWidth() / 2) - (img.getWidth() / 2), FONT_HEIGHT + INFO_PADDING, img.getWidth(), animSize, xMin, yMin, img.getWidth(), animSize, false);
	g.setFont(FONT);
	g.setFont(FONT_HEIGHT);
	g.setColour(INFO_COLOR);
	g.drawSingleLineText(title, sliderBounds.getWidth() / 2, FONT_HEIGHT, juce::Justification::horizontallyCentred);

	LookAndFeel_V4::drawRotarySlider(g, x, y, width, height, sliderPos, rotaryStartAngle, rotaryEndAngle, slider);
}

void BitmapSwitchLookAndFeel::drawLabel(Graphics& g, Label& label) {
	g.setColour(INFO_COLOR);
	g.drawRoundedRectangle(0, 0, label.getWidth(), label.getHeight(), 8, 2);
	LookAndFeel_V4::drawLabel(g, label);
}

Slider::SliderLayout BitmapSwitchLookAndFeel::getSliderLayout(Slider& slider) {
	LookAndFeel_V4::getSliderLayout(slider);
	Slider::SliderLayout layout;
	layout.textBoxBounds = textBounds;
	layout.sliderBounds = sliderBounds;
	return layout;
}

BitmapSwitch::BitmapSwitch() {
	setLookAndFeel(&lf);
	setSliderStyle(juce::Slider::SliderStyle::Rotary);
	setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
	setColour(juce::Slider::thumbColourId, juce::Colour());
	setColour(juce::Slider::rotarySliderFillColourId, juce::Colour());
	setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour());
}

void BitmapSwitch::setState(Image img, int animSize, std::string title) {
	if (animSize <= 0) return;
	lf.setBitmapState(img, animSize, title);
}

void BitmapSwitch::mouseDown(const MouseEvent& e) {
	lf.incBitmapFrame();
	this->setValue(lf.getAnimPercent(), sendNotificationSync);
	getParentComponent()->repaint();
}

void BitmapSwitch::mouseUp(const MouseEvent& e) {
	if(isMomentary) lf.incBitmapFrame();
	this->setValue(lf.getAnimPercent(), sendNotificationSync);
	getParentComponent()->repaint();
}

int BitmapSwitch::getState() {
	return state;
}

void BitmapSwitch::setBounds(int x, int y, int width, int height) {
	lf.setBounds(x, y, width, height);
	Component::setBounds(x, y, width, height);
}
/*
  ==============================================================================

    BitmapKnob.cpp
    Created: 16 Nov 2021 8:29:00pm
    Author:  epici

  ==============================================================================
*/

#include "BitmapKnob.h"
#include "PluginConstants.h"

void BitmapKnobLookAndFeel::setBitmapState(Image img, std::string title) {
    this->img = img;
    this->title = title;
    sliderBounds = Rectangle<int>(0, 0, img.getWidth(), img.getHeight());
    textBounds = Rectangle<int>(0, FONT_HEIGHT + INFO_PADDING + img.getHeight() + INFO_PADDING, img.getWidth(), 20);
}

void BitmapKnobLookAndFeel::drawLinearSlider(Graphics& g, int x, int y, int width, int height,
    float sliderPos, float minSliderPos, float maxSliderPos, const Slider::SliderStyle style, Slider& slider) {

    //LookAndFeel_V4::drawLinearSlider(g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);

    g.setColour(Colour(0x24, 0x26, 0x27));
    g.setOpacity(0.6);
    g.fillRoundedRectangle(0.0f, (float)height / 3.0, (float)width, (float)height / 2.0, 2.0);
    g.setOpacity(1.0);

    float psuedoTrackLen = (float)width - (float)img.getHeight();
    float realTrackLen = width;

    sliderPos = sliderPos * (psuedoTrackLen / realTrackLen);
    
    juce::AffineTransform transform;
    float rotateAmt = slider.getRotaryParameters().endAngleRadians - slider.getRotaryParameters().startAngleRadians;
    transform = transform.rotated(MathConstants<float>::halfPi, (float)(img.getWidth() / 2), (float)(img.getHeight() / 2));
    float choppedDiff = (img.getWidth() - img.getHeight()) / 2;
    transform = transform.translated(sliderPos - choppedDiff - 1, 0);
    g.drawImageTransformed(img, transform, false);
}

void  BitmapKnobLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
    const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider) {

    LookAndFeel_V4::drawRotarySlider(g, x, y, width, height, sliderPos, rotaryStartAngle, rotaryEndAngle, slider);
    juce::AffineTransform transform;
    float rotateAmt = slider.getRotaryParameters().endAngleRadians - slider.getRotaryParameters().startAngleRadians;
    transform = transform.rotated(sliderPos * rotateAmt, (float)(img.getWidth() / 2), (float)(img.getHeight() / 2));
    transform = transform.translated(0, FONT_HEIGHT + INFO_PADDING);
    g.setColour(Colour(0, 0, 0));
    g.drawImageTransformed(img, transform, false);
    g.setFont(FONT);
    g.setFont(FONT_HEIGHT);
    g.setColour(INFO_COLOR);
    g.drawSingleLineText(title, width / 2, FONT_HEIGHT, juce::Justification::horizontallyJustified);
}

void BitmapKnobLookAndFeel::drawLabel(Graphics& g, Label& label) {
    g.setColour(Colour(0x7B, 0x8F, 0x95));
    g.drawRoundedRectangle(0,0, label.getWidth(), label.getHeight(), 2, 5);
    LookAndFeel_V4::drawLabel(g, label);
}

Slider::SliderLayout BitmapKnobLookAndFeel::getSliderLayout(Slider& slider) {
    LookAndFeel_V4::getSliderLayout(slider);
    Slider::SliderLayout layout = LookAndFeel_V4::getSliderLayout(slider);
    if (slider.getSliderStyle() == Slider::SliderStyle::Rotary) {
        layout.textBoxBounds = textBounds;
        layout.sliderBounds = sliderBounds;
    } 
    return layout;
}

BitmapKnob::BitmapKnob() {
    setLookAndFeel(&lf);

    setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 50, 20);

    setColour(juce::Slider::thumbColourId, juce::Colour());
    setColour(juce::Slider::rotarySliderFillColourId, juce::Colour());
    setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour());
    setColour(juce::Slider::textBoxOutlineColourId, juce::Colour());
    setColour(juce::Slider::textBoxTextColourId, juce::Colour((juce::uint8)0x20, (juce::uint8)0x20, (juce::uint8)0x20, (juce::uint8)0xFF));
}

void BitmapKnob::setBitmap(Image img, std::string title) {
    lf.setBitmapState(img, title);
    if (getSliderStyle() == Slider::Rotary) {
        setBounds(getBounds().getX(), getBounds().getY(), img.getWidth(), img.getHeight() + FONT_HEIGHT + INFO_PADDING + TEXTBOX_SIZE + INFO_PADDING);
    }
}

void BitmapKnob::setXY(int x, int y) {
    setBounds(x, y, getBounds().getWidth(), getBounds().getHeight());
}
/*
  ==============================================================================

    BitmapKnob.h
    Created: 16 Nov 2021 8:29:00pm
    Author:  epici

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <string>

using namespace juce;

class BitmapKnobLookAndFeel : public LookAndFeel_V4
{
    Image img;
    Rectangle<int> textBounds;
    Rectangle<int> sliderBounds;
    std::string title;
public:
    void setBitmapState(Image img, std::string title);
    Slider::SliderLayout getSliderLayout(Slider& slider) override;
    void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
        const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&) override;
    void drawLinearSlider(Graphics&, int x, int y, int width, int height,
        float sliderPos, float minSliderPos, float maxSliderPos,
        const Slider::SliderStyle, Slider&) override;
    void drawLabel(Graphics& g, Label& label) override;
};

struct BitmapKnob : public Slider {
    BitmapKnobLookAndFeel lf;
public:
    BitmapKnob();
    void setBitmap(Image img, std::string title);
    void setXY(int x, int y);
};
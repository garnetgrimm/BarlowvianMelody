#pragma once

#include <JuceHeader.h>
#include <string>
#include <vector>

using namespace juce;
using namespace std;

class BitmapSwitchLookAndFeel : public LookAndFeel_V4
{
    Image img;
    std::string title;
    int frame = 0;
    int animSize = 0;
public:
    Rectangle<int> textBounds;
    Rectangle<int> sliderBounds;
    void setBounds(int x, int y, int width, int height);
    void incBitmapFrame();
    int getBitmapFrame();
    float getAnimPercent();
    void setBitmapState(Image img, int animSize, std::string title);
    Slider::SliderLayout getSliderLayout(Slider& slider) override;
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&) override;
    void drawLabel(Graphics& g, Label& label) override;
};

class BitmapSwitch : public Slider
{
public:
    BitmapSwitchLookAndFeel lf;
    BitmapSwitch();
    void setState(Image img, int animSize, std::string title);
    void mouseDown(const MouseEvent& e) override;
    void mouseUp(const MouseEvent& e) override;
    void mouseMove(const MouseEvent&) override {};
    void mouseExit(const MouseEvent&) override {};
    void mouseEnter(const MouseEvent&) override {};
    int getState();
    void setBounds(int x, int y, int width, int height);
    bool isMomentary = false;
private:
    int state = 0;
};
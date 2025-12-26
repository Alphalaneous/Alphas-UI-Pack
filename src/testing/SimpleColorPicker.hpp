#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class SimpleColorPicker : public cocos2d::CCLayer, public cocos2d::extension::ColorPickerDelegate {
    std::function<void(const cocos2d::ccColor4B&)> m_callback;
    cocos2d::extension::CCControlColourPicker* m_colorPicker;
    GLubyte m_opacity;
public:
    static SimpleColorPicker* create(const cocos2d::ccColor4B& color, std::function<void(const cocos2d::ccColor4B&)> callback);
    bool init(const cocos2d::ccColor4B& color, std::function<void(const cocos2d::ccColor4B&)> callback);

    ccColor3B cc4Bto3B(ccColor4B color);
    ccColor4B cc3Bto4B(ccColor3B color);

    void colorValueChanged(cocos2d::ccColor3B color) override;
    void setColor(const cocos2d::ccColor4B& color);
    ccColor4B getColor();
};
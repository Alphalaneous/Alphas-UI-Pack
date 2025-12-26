#include "SimpleColorPicker.hpp"

ccColor3B SimpleColorPicker::cc4Bto3B(ccColor4B color) {
    return {color.r, color.g, color.b};
}

ccColor4B SimpleColorPicker::cc3Bto4B(ccColor3B color) {
    return {color.r, color.g, color.b, m_opacity};
}

SimpleColorPicker* SimpleColorPicker::create(const cocos2d::ccColor4B& color, std::function<void(const cocos2d::ccColor4B&)> callback){
    auto ret = new SimpleColorPicker();
    if (ret->init(color, callback)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

bool SimpleColorPicker::init(const cocos2d::ccColor4B& color, std::function<void(const cocos2d::ccColor4B&)> callback) {

    m_callback = std::move(callback);
    m_opacity = color.a;
    m_colorPicker = CCControlColourPicker::colourPicker();
    m_colorPicker->setScale(0.8f);
    m_colorPicker->setColorValue(cc4Bto3B(color));
    m_colorPicker->setDelegate(this);
    m_colorPicker->ignoreAnchorPointForPosition(false);
    m_colorPicker->setID("color-picker");

    ignoreAnchorPointForPosition(false);

    auto pickerSize = m_colorPicker->getScaledContentSize();

    setContentSize(pickerSize);
    auto size = getScaledContentSize();

    m_colorPicker->setPosition(getContentSize());

    addChild(m_colorPicker);
    return true;
}

void SimpleColorPicker::colorValueChanged(cocos2d::ccColor3B color) {
    if (m_callback) m_callback(cc3Bto4B(color));
}

void SimpleColorPicker::setColor(const cocos2d::ccColor4B& color) {
    m_colorPicker->setColorValue(cc4Bto3B(color));
}

ccColor4B SimpleColorPicker::getColor() {
    return cc3Bto4B(m_colorPicker->getColorValue());
}

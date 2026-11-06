#include <Geode/Geode.hpp>
#include "API.hpp"
#include "Geode/cocos/CCDirector.h"

using namespace geode::prelude;
using namespace alpha::prelude;

struct Touch::Impl final {
	geode::MouseInputData::Button m_button;
    Ref<CCTouch> m_touch = nullptr;
};

Touch::Touch() : m_impl(std::make_unique<Impl>()) {}
Touch::~Touch() = default;

Touch* Touch::create(geode::MouseInputData::Button button) {
    auto ret = new Touch();
    if (ret->init(button)){
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool Touch::init(geode::MouseInputData::Button button) {
    m_impl->m_button = button;
    return true;
}

void Touch::setOriginalTouch(CCTouch* touch) {
    m_impl->m_touch = touch;
}

CCTouch* Touch::getOriginalTouch() {
    return m_impl->m_touch;
}

geode::MouseInputData::Button Touch::getButton() {
    return m_impl->m_button;
}
#include <Geode/Geode.hpp>
#include "API.hpp"

using namespace geode::prelude;
using namespace alpha::prelude;

struct Touch::Impl final {
	geode::MouseInputData::Button m_button;
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

geode::MouseInputData::Button Touch::getButton() {
    return m_impl->m_button;
}

void Touch::setTouchInfo(const CCPoint& point) {
    m_prevPoint = m_point;
    m_point = CCPoint{point.x, point.y};
    if (!m_startPointCaptured) {
        m_startPoint = m_point;
        m_startPointCaptured = true;
    }
}
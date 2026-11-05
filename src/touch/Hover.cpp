#include <Geode/Geode.hpp>
#include "API.hpp"

using namespace geode::prelude;
using namespace alpha::prelude;

Hover* Hover::create() {
    auto ret = new Hover();
    ret->autorelease();
    return ret;
}

void Hover::setTouchInfo(const CCPoint& point) {
    m_prevPoint = m_point;
    m_point = CCPoint{point.x, point.y};
    if (!m_startPointCaptured) {
        m_startPoint = m_point;
        m_startPointCaptured = true;
    }
}
#include <Geode/Geode.hpp>
#include "API.hpp"

using namespace geode::prelude;
using namespace alpha::prelude;

Hover* Hover::create() {
    auto ret = new Hover();
    ret->autorelease();
    return ret;
}
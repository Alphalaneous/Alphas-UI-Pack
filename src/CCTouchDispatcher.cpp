#include <Geode/Geode.hpp>
#include <Geode/modify/CCTouchDispatcher.hpp>
#include "nodes/scroll/AdvancedScrollLayer.hpp"
#include "Utils.hpp"

using namespace geode::prelude;

static std::set<CCTouchDelegate*> s_removedDelegates;
static std::unordered_map<CCTouch*, alpha::ui::AdvancedScrollLayer*> s_touchScrollLayers;

class $modify(AUPCCTouchDispatcher, CCTouchDispatcher) {

    static void onModify(auto& self) {
        (void) self.setHookPriorityPre("CCTouchDispatcher::touches", Priority::VeryEarlyPre);
    }

    CCArray* filterHandlers(alpha::ui::AdvancedScrollLayer* preferredScroll, CCTouch* touch, unsigned int index) {
        auto removed = CCArray::create();

        for (auto handler : CCArrayExt<CCTargetedTouchHandler, false>(m_pTargetedHandlers)) {
            if (preferredScroll && handler->getDelegate() == preferredScroll) continue;

            auto node = typeinfo_cast<CCNode*>(handler->getDelegate());

            if (!node) {
                if (preferredScroll) {
                    removed->addObject(handler);
                }
                continue;
            }

            auto scroll = node->getParentByType<alpha::ui::AdvancedScrollLayer>();

            if (preferredScroll && scroll && (scroll != preferredScroll)) {
                removed->addObject(handler);
                continue;
            }

            if (scroll) {
                if (!preferredScroll || (index == CCTOUCHBEGAN && !alpha::utils::isPointInsideNode(scroll, touch->getLocation()))) {
                    removed->addObject(handler);
                    continue;
                }
            }
        }

        m_pTargetedHandlers->removeObjectsInArray(removed);

        return removed;
    }

    void restoreHandlers(CCArray* handlers) {
        for (auto handler : CCArrayExt<CCTargetedTouchHandler, false>(handlers)) {
            if (s_removedDelegates.contains(handler->getDelegate())) continue;

            bool exists = false;

            for (auto existing : CCArrayExt<CCTargetedTouchHandler, false>(m_pTargetedHandlers)) {
                if (existing == handler) {
                    exists = true;
                    break;
                }
            }

            if (!exists) {
                m_pTargetedHandlers->addObject(handler);
            }
        }
    }

    void dispatchTouch(CCTouch* touch, CCEvent* event, unsigned int index, alpha::ui::AdvancedScrollLayer* preferredScroll) {
        if (!touch) return;

        auto removed = filterHandlers(preferredScroll, touch, index);

        auto touchSet = CCSet::create();
        touchSet->addObject(touch);

        CCTouchDispatcher::touches(touchSet, event, index);

        restoreHandlers(removed);
    }

    void removeDelegate(CCTouchDelegate* delegate) {
        s_removedDelegates.insert(delegate);
        CCTouchDispatcher::removeDelegate(delegate);
    }

    void addTargetedDelegate(CCTouchDelegate *delegate, int priority, bool swallowsTouches) {
        s_removedDelegates.erase(delegate);
        CCTouchDispatcher::addTargetedDelegate(delegate, priority, swallowsTouches);
    }

    void touches(CCSet* touches, CCEvent* event, unsigned int index) {
        if (!touches || touches->count() == 0) return;

        auto touch = static_cast<CCTouch*>(touches->anyObject());

        if (index == CCTOUCHBEGAN) {
            auto removed = filterHandlers(nullptr, touch, index);
            CCTouchDispatcher::touches(touches, event, index);

            for (auto handler : CCArrayExt<CCTargetedTouchHandler, false>(m_pTargetedHandlers)) {
                auto claimed = handler->getClaimedTouches();
                if (claimed && !claimed->containsObject(touch)) continue;

                auto scroll = typeinfo_cast<alpha::ui::AdvancedScrollLayer*>(handler->getDelegate());

                if (scroll) {
                    s_touchScrollLayers[touch] = scroll;
                    break;
                }
            }
            restoreHandlers(removed);
        }

        auto it = s_touchScrollLayers.find(touch);

        if (it != s_touchScrollLayers.end()) {
            auto scroll = it->second;

            dispatchTouch(touch, event, index, scroll);

            if (index == CCTOUCHENDED || index == CCTOUCHCANCELLED) {
                s_touchScrollLayers.erase(it);
            }
        }
        else if (index != CCTOUCHBEGAN) {
            bool blocked = false;
            for (const auto& [k, v] : s_touchScrollLayers) {
                if (v->blocksTouchBehind()) {
                    blocked = true;
                    break;
                }
            }

            if (!blocked) {
                auto removed = filterHandlers(nullptr, touch, index);
                CCTouchDispatcher::touches(touches, event, index);
                restoreHandlers(removed);
            }
        }

        s_removedDelegates.clear();
    }
};
#include <Geode/Geode.hpp>
#include <Geode/modify/CCTouchDispatcher.hpp>
#include "nodes/scroll/AdvancedScrollLayer.hpp"

using namespace geode::prelude;

static std::set<CCTouchDelegate*> s_removedDelegates;

class $modify(AUPCCTouchDispatcher, CCTouchDispatcher) {

    static void onModify(auto& self) {
        (void) self.setHookPriorityPre("CCTouchDispatcher::touches", Priority::VeryEarlyPre);
    }

    CCArray* filterHandlers(alpha::ui::AdvancedScrollLayer* preferredScroll) {
        auto removed = CCArray::create();

        for (auto handler : CCArrayExt<CCTargetedTouchHandler, false>(m_pTargetedHandlers)) {
            if (preferredScroll && handler->getDelegate() == preferredScroll) continue;

            auto node = typeinfo_cast<CCNode*>(handler->getDelegate());
            auto scroll = node ? node->getParentByType<alpha::ui::AdvancedScrollLayer>() : nullptr;

            if ((!preferredScroll && scroll) || (preferredScroll && !node)) {
                removed->addObject(handler);
            }
        }

        m_pTargetedHandlers->removeObjectsInArray(removed);
        return removed;
    }

    void restoreHandlers(CCArray* handlers) {
        for (auto handler : CCArrayExt<CCTargetedTouchHandler, false>(handlers)) {
            if (s_removedDelegates.contains(handler->getDelegate())) continue;

            bool exists = m_pTargetedHandlers->containsObject(handler);
            if (exists) continue;

            m_pTargetedHandlers->addObject(handler);
        }
    }

    void dispatchTouch(CCTouch* touch, CCEvent* event, unsigned int index, alpha::ui::AdvancedScrollLayer* preferredScroll) {
        if (!touch) return;

        auto removed = filterHandlers(preferredScroll);

        auto touchSet = CCSet::create();
        touchSet->addObject(touch);

        CCTouchDispatcher::touches(touchSet, event, index);

        restoreHandlers(removed);
    }

    void removeDelegate(CCTouchDelegate* delegate) {
        s_removedDelegates.insert(delegate);
        CCTouchDispatcher::removeDelegate(delegate);
    }

    void addTargetedDelegate(CCTouchDelegate* delegate, int priority, bool swallowsTouches) {
        s_removedDelegates.erase(delegate);
        CCTouchDispatcher::addTargetedDelegate(delegate, priority, swallowsTouches);
    }

    void cancelAllTouches(CCTouch* touch, CCEvent* event, unsigned int index, alpha::ui::AdvancedScrollLayer* scrollLayer) {
        for (auto handler : CCArrayExt<CCTargetedTouchHandler, false>(m_pTargetedHandlers)) {
            auto claimed = handler->getClaimedTouches();
            if (!claimed || !claimed->containsObject(touch)) continue;

            auto delegate = handler->getDelegate();
            if (!delegate || delegate == scrollLayer) continue;

            delegate->ccTouchCancelled(touch, event);
            claimed->removeObject(touch);
        }
    }

    void touches(CCSet* touches, CCEvent* event, unsigned int index) {
        if (!touches || touches->count() == 0) return;

        auto touch = static_cast<CCTouch*>(touches->anyObject());

        if (index == CCTOUCHBEGAN) {
            alpha::ui::AdvancedScrollLayer* scrollLayer = nullptr;

            auto removed = filterHandlers(nullptr);
            CCTouchDispatcher::touches(touches, event, index);

            for (auto handler : CCArrayExt<CCTargetedTouchHandler, false>(m_pTargetedHandlers)) {
                auto claimed = handler->getClaimedTouches();
                if (!claimed || !claimed->containsObject(touch)) continue;

                auto scroll = typeinfo_cast<alpha::ui::AdvancedScrollLayer*>(handler->getDelegate());
                if (!scroll) continue;

                scrollLayer = scroll;
                break;
            }
            
            restoreHandlers(removed);

            if (scrollLayer) {
                cancelAllTouches(touch, event, index, scrollLayer);
                dispatchTouch(touch, event, index, scrollLayer);
            }

            s_removedDelegates.clear();
            return;
        }

        CCTouchDispatcher::touches(touches, event, index);
        s_removedDelegates.clear();
    }
};
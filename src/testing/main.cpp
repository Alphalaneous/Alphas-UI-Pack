#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "ScrollTestLayer.hpp"

using namespace geode::prelude;
using namespace alpha::prelude;

/*class $modify(MenuLayer) {

	bool init() {
		if (!MenuLayer::init()) return false;

		auto bottomMenu = getChildByID("bottom-menu");

		auto myButton = CCMenuItemSpriteExtra::create(
			CCSprite::createWithSpriteFrameName("GJ_likeBtn_001.png"),
			this,
			menu_selector(Self::onScrollLayerTest)
		);

		bottomMenu->addChild(myButton);
		bottomMenu->updateLayout();

		auto mainMenu = getChildByID("main-menu");

		auto mainMenuRender = RenderNode::create(mainMenu);
		auto bottomMenuRender = RenderNode::create(bottomMenu);

		mainMenu->removeFromParentAndCleanup(false);
		bottomMenu->removeFromParentAndCleanup(false);

		CCNode* empty = CCNode::create();

		addChild(empty);

		empty->updateLayout();

		addChild(mainMenuRender);
		addChild(bottomMenuRender);

		return true;
	}

	void onScrollLayerTest(CCObject* sender) {
		auto testLayer = ScrollTestLayer::create();
		testLayer->show();
	}
};*/

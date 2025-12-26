#pragma once

#include "API.hpp"

class ArtNode : public cocos2d::CCLayerColor, public alpha::dispatcher::TouchDelegate {
public:
	static ArtNode* create(const cocos2d::CCSize& size);

	bool init(const cocos2d::CCSize& size);

	void drawRectBetweenPoints(const cocos2d::CCPoint& a, const cocos2d::CCPoint& b, float width, const cocos2d::ccColor4B& color);
	void addPoint(const cocos2d::CCPoint& p);
	void setHoverColor(const cocos2d::ccColor3B& color);

	virtual bool clickBegan(alpha::dispatcher::TouchEvent* touch) override;
    virtual void clickMoved(alpha::dispatcher::TouchEvent* touch) override;
    virtual void clickEnded(alpha::dispatcher::TouchEvent* touch) override;
	virtual bool mouseEntered(alpha::dispatcher::TouchEvent* touch) override;
	virtual void mouseExited(alpha::dispatcher::TouchEvent* touch) override;
	virtual void mouseMoved(alpha::dispatcher::TouchEvent* touch) override;
	virtual void onEnter() override;
	virtual void onExit() override;


	cocos2d::CCRenderTexture* m_canvas = nullptr;
	cocos2d::CCPoint m_lastPoint;
    float m_brushSize = 10.f;
	bool m_hasLastPoint = false;

	bool m_holding = false;
	cocos2d::ccColor4B m_clickColor {0, 0, 0, 255};
};
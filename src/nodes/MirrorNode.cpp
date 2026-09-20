#include "nodes/MirrorNode.hpp"
#include <Geode/Geode.hpp>
#include "API.hpp"
#include "../CCTexture2DExt.hpp"
#include "Image.hpp"

using namespace geode::prelude;
using namespace alpha::prelude;

struct MirrorNode::Impl final {
    WeakRef<CCNode> m_nodeToRender;
    Ref<CCNode> m_owned;
    GLuint m_fbo = 0;
    GLuint m_rbo = 0;
    GLuint m_texture = 0;
    CCSize m_size;
    int m_texWidth = 0;
    int m_texHeight = 0;
    int m_touchPriority = 0;
    bool m_showOriginal = true;
    bool m_ownsNode = false;
    bool m_additionalTransformDirty;
    cocos2d::CCAffineTransform m_additionalTransform = CCAffineTransformMakeIdentity();
    cocos2d::CCAffineTransform m_compoundTransform = CCAffineTransformMakeIdentity();
};

MirrorNode::MirrorNode() : m_impl(std::make_unique<Impl>()) {}

MirrorNode::~MirrorNode() {
    if (m_impl->m_fbo) glDeleteFramebuffers(1, &m_impl->m_fbo);
    #ifndef GEODE_IS_IOS
    if (m_impl->m_rbo) glDeleteRenderbuffers(1, &m_impl->m_rbo);
    #endif
    // CCTexture2D will delete the texture for me
}

MirrorNode* MirrorNode::create(CCNode* node, bool ownsNode) {
    auto ret = new MirrorNode();
    if (ret->init(node, ownsNode)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool MirrorNode::init(CCNode* node, bool ownsNode) {
    if (!CCSprite::init()) return false;
    m_impl->m_size = CCDirector::get()->getWinSize();

    setNode(node, ownsNode);

    scheduleUpdate();
    return true;
}

void MirrorNode::setNode(CCNode* node, bool ownsNode) {
    m_impl->m_nodeToRender = node;
    m_impl->m_ownsNode = ownsNode;
    m_impl->m_owned = ownsNode ? node : nullptr;

    resetFBO();
}

void MirrorNode::setOwnsNode(bool ownsNode) {
    m_impl->m_ownsNode = ownsNode;
    m_impl->m_owned = ownsNode ? m_impl->m_nodeToRender.lock() : nullptr;
}

bool MirrorNode::ownsNode() {
    return m_impl->m_ownsNode;
}

void MirrorNode::setViewportSize(const cocos2d::CCSize& size) {
    m_impl->m_size = size;
    resetFBO();
}

const cocos2d::CCSize& MirrorNode::getViewportSize() {
    return m_impl->m_size;
}

void MirrorNode::setNodeTransform(const cocos2d::CCAffineTransform& additionalTransform) {
    m_impl->m_additionalTransform = additionalTransform;
    m_impl->m_additionalTransformDirty = true;
}

const cocos2d::CCAffineTransform& MirrorNode::getNodeTransform() {
    return m_impl->m_additionalTransform;
}

void MirrorNode::initFBO() {
    auto node = m_impl->m_nodeToRender.lock();
    if (!node) return;

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    auto scale = CCDirector::get()->getContentScaleFactor();

    m_impl->m_texWidth = m_impl->m_size.width * scale;
    m_impl->m_texHeight = m_impl->m_size.height * scale;

    glGenTextures(1, &m_impl->m_texture);
    glBindTexture(GL_TEXTURE_2D, m_impl->m_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_impl->m_texWidth, m_impl->m_texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLint prevFbo = 0;
    GLint prevRbo = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevFbo);
    #ifndef GEODE_IS_IOS
    glGetIntegerv(GL_RENDERBUFFER_BINDING, &prevRbo);
    #endif

    glGenFramebuffers(1, &m_impl->m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_impl->m_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_impl->m_texture, 0);

    #ifndef GEODE_IS_IOS
    glGenRenderbuffers(1, &m_impl->m_rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, m_impl->m_rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_impl->m_texWidth, m_impl->m_texHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, 0x821A, GL_RENDERBUFFER, m_impl->m_rbo);
    #endif

    glBindFramebuffer(GL_FRAMEBUFFER, prevFbo);

    #ifndef GEODE_IS_IOS
    glBindFramebuffer(GL_RENDERBUFFER, prevRbo);
    #endif

    auto texObj = CCTexture2DExt::create(m_impl->m_texture, m_impl->m_texWidth, m_impl->m_texHeight, CCSize(m_impl->m_texWidth / scale, m_impl->m_texHeight / scale));

    setTexture(texObj);
    setTextureRect({0.f, 0.f, m_impl->m_size.width, m_impl->m_size.height});
    setContentSize(m_impl->m_size);
}

void MirrorNode::resetFBO() {
    if (m_impl->m_fbo) glDeleteFramebuffers(1, &m_impl->m_fbo);
    #ifndef GEODE_IS_IOS
    if (m_impl->m_rbo) glDeleteRenderbuffers(1, &m_impl->m_rbo);
    #endif
    m_impl->m_fbo = 0;
    m_impl->m_rbo = 0;

    render();
}

void MirrorNode::render() {
    auto node = m_impl->m_nodeToRender.lock();
    if (!node) return;

    if (!m_impl->m_fbo) initFBO();
    auto bounds = node->boundingBox();
    setTextureRect(getTextureRect(), m_bRectRotated, bounds.size);

    GLint oldFBO = 0;
    GLint oldRBO = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldFBO);
    #ifndef GEODE_IS_IOS
    glGetIntegerv(GL_RENDERBUFFER_BINDING, &oldRBO);
    #endif

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    glBindFramebuffer(GL_FRAMEBUFFER, m_impl->m_fbo);
    glViewport(0, 0, m_impl->m_texWidth, m_impl->m_texHeight);

    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    kmGLMatrixMode(KM_GL_PROJECTION);
    kmGLPushMatrix();
    kmGLLoadIdentity();

    auto scale = CCDirector::get()->getContentScaleFactor();

    kmMat4 ortho;
    kmMat4OrthographicProjection(&ortho, 0, m_impl->m_size.width, m_impl->m_size.height, 0, -1024, 1024);

    kmGLMultMatrix(&ortho);

    kmGLMatrixMode(KM_GL_MODELVIEW);
    kmGLPushMatrix();
    kmGLLoadIdentity();

    kmGLTranslatef(
        m_impl->m_size.width / 2.f - (bounds.origin.x + bounds.size.width / 2.f),
        m_impl->m_size.height / 2.f - (bounds.origin.y + bounds.size.height / 2.f),
        0.f
    );

    if (m_impl->m_additionalTransformDirty || node->m_bAdditionalTransformDirty) {
        m_impl->m_compoundTransform = CCAffineTransformConcat(node->m_sAdditionalTransform, m_impl->m_additionalTransform);
    }

    auto oldAdditionalTransform = node->m_sAdditionalTransform;

    node->setVisible(true);
    node->setAdditionalTransform(m_impl->m_compoundTransform);
    
    node->visit();

    node->setVisible(showingOriginal());
    node->setAdditionalTransform(oldAdditionalTransform);

    kmGLPopMatrix();
    kmGLMatrixMode(KM_GL_PROJECTION);
    kmGLPopMatrix();
    kmGLMatrixMode(KM_GL_MODELVIEW);

    glBindFramebuffer(GL_FRAMEBUFFER, oldFBO);
    #ifndef GEODE_IS_IOS
    glBindFramebuffer(GL_RENDERBUFFER, oldRBO);
    #endif
    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
}

void MirrorNode::update(float dt) {
    render();
}

CCNode* MirrorNode::getNode() {
    return m_impl->m_nodeToRender.lock();
}

void MirrorNode::onEnter() {
    CCSprite::onEnter();
    registerWithTouchDispatcher();
}

void MirrorNode::onExit() {
    CCSprite::onExit();
    CCTouchDispatcher::get()->removeDelegate(this);
}

void MirrorNode::registerWithTouchDispatcher() {
    CCTouchDispatcher::get()->addTargetedDelegate(this, m_impl->m_touchPriority, true);
}

void MirrorNode::setTouchPriority(int priority) {
    m_impl->m_touchPriority = priority;
}

int MirrorNode::getTouchPriority() {
    return m_impl->m_touchPriority;
}

void MirrorNode::showOriginal(bool show) {
    m_impl->m_showOriginal = show;
}

bool MirrorNode::showingOriginal() {
    return m_impl->m_showOriginal;
}

bool MirrorNode::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) {
    bool ret = false;

    forwardTouch<CCTouchDelegate>([touch, event, &ret] (CCTouchDelegate* delegate) {
        ret = delegate->ccTouchBegan(touch, event);
    });

    return ret;
}

void MirrorNode::ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) {
    forwardTouch<CCTouchDelegate>([touch, event] (CCTouchDelegate* delegate) {
        delegate->ccTouchMoved(touch, event);
    });
}

void MirrorNode::ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) {
    forwardTouch<CCTouchDelegate>([touch, event] (CCTouchDelegate* delegate) {
        delegate->ccTouchEnded(touch, event);
    });
}

void MirrorNode::ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) {
    forwardTouch<CCTouchDelegate>([touch, event] (CCTouchDelegate* delegate) {
        delegate->ccTouchCancelled(touch, event);
    });
}

bool MirrorNode::mouseEntered(dispatcher::TouchEvent* touch) {
    bool ret = false;

    forwardTouch<dispatcher::TouchDelegate>([touch, &ret] (dispatcher::TouchDelegate* delegate) {
        ret = delegate->mouseEntered(touch);
    });

    return ret;
}

void MirrorNode::mouseExited(dispatcher::TouchEvent* touch) {
    forwardTouch<dispatcher::TouchDelegate>([touch] (dispatcher::TouchDelegate* delegate) {
        delegate->mouseExited(touch);
    });
}

void MirrorNode::mouseMoved(dispatcher::TouchEvent* touch) {
    forwardTouch<dispatcher::TouchDelegate>([touch] (dispatcher::TouchDelegate* delegate) {
        delegate->mouseMoved(touch);
    });
}

cocos2d::CCTexture2D* MirrorNode::captureTexture() {
    if (!m_impl->m_fbo) {
        return nullptr;
    }

    GLint oldFBO = 0;
    GLint oldTexture = 0;

    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldFBO);
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &oldTexture);

    glBindFramebuffer(GL_FRAMEBUFFER, m_impl->m_fbo);

    GLuint texture = 0;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_impl->m_texWidth, m_impl->m_texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, m_impl->m_texWidth, m_impl->m_texHeight);

    glBindTexture(GL_TEXTURE_2D, oldTexture);
    glBindFramebuffer(GL_FRAMEBUFFER, oldFBO);

    auto scale = CCDirector::get()->getContentScaleFactor();

    return CCTexture2DExt::create(texture, m_impl->m_texWidth, m_impl->m_texHeight, CCSize(m_impl->m_texWidth / scale, m_impl->m_texHeight / scale));
}

cocos2d::CCSprite* MirrorNode::captureSprite() {
    auto spr = CCSprite::createWithTexture(captureTexture());
    spr->setTextureRect(getTextureRect(), m_bRectRotated, boundingBox().size);

    return spr;
}

Image* MirrorNode::captureImage() {
    auto node = m_impl->m_nodeToRender.lock();
    if (!node) return nullptr;

    auto bounds = node->boundingBox();
    auto scale = CCDirector::get()->getContentScaleFactor();

    auto width = static_cast<GLint>(bounds.size.width * scale);
    auto height = static_cast<GLint>(bounds.size.height * scale);

    auto x = (m_impl->m_texWidth - width) / 2;
    auto y = (m_impl->m_texHeight - height) / 2;

    GLint oldFBO = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldFBO);

    glBindFramebuffer(GL_FRAMEBUFFER, m_impl->m_fbo);

    std::vector<ccColor4B> data(width * height);

    glReadPixels(
        x, y,
        width, height,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        data.data()
    );

    glBindFramebuffer(GL_FRAMEBUFFER, oldFBO);

    return Image::create(width, height, std::move(data));
}
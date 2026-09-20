#include <Geode/Geode.hpp>

using namespace geode::prelude;

class CCTexture2DExt : public CCTexture2D {
public:
    static CCTexture2DExt* create(GLuint name, GLsizei pixelsWidth, GLsizei pixelsHeight, const CCSize& contentSize) {
        auto ret = new CCTexture2DExt();
        if (ret->initWithGLName(name, pixelsWidth, pixelsHeight, contentSize)) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }

protected:
    bool initWithGLName(GLuint name, GLsizei pixelsWidth, GLsizei pixelsHeight, const CCSize& contentSize) {
        m_uName = name;

        m_tContentSize = contentSize;
        m_uPixelsWide = pixelsWidth;
        m_uPixelsHigh = pixelsHeight;
        m_ePixelFormat = kCCTexture2DPixelFormat_RGBA8888;
        m_fMaxS = contentSize.width / static_cast<float>(pixelsWidth);
        m_fMaxT = contentSize.height / static_cast<float>(pixelsHeight);

        m_bHasPremultipliedAlpha = true;
        m_bHasMipmaps = false;

        setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTexture));
        return true;
    }
};
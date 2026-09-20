#pragma once

#include <Geode/cocos/cocoa/CCObject.h>
#include <filesystem>

#include "export.hpp"

namespace alpha::ui {

class ALPHAS_UI_PACK_DLL Image : public cocos2d::CCObject {
public:
    Image();
    ~Image();

    /// Create Image with RGBA ccColor4B data
    static Image* create(unsigned int width, unsigned int height, std::vector<cocos2d::ccColor4B> data);

    /// Get Image Width
    unsigned int getWidth() const;

    /// Get Image Height
    unsigned int getHeight() const;

    /// Save image to file as PNG
    geode::Result<> saveToFile(const std::filesystem::path& path);

    /// Get image pixels
    std::span<cocos2d::ccColor4B> getPixels();

    /// Get raw image bytes
    std::span<std::byte> bytes();

protected:
    struct Impl;
    std::unique_ptr<Impl> m_impl; 
};

}
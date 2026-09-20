#include "Image.hpp"
#include <Geode/Geode.hpp>
#include "API.hpp"
#include <spng.h>

using namespace geode::prelude;
using namespace alpha::prelude;

struct Image::Impl final {
    std::vector<ccColor4B> m_data;
    unsigned int m_width;
    unsigned int m_height;
};

Image::Image() : m_impl(std::make_unique<Impl>()) {}
Image::~Image() {}

Image* Image::create(unsigned int width, unsigned int height, std::vector<ccColor4B> data) {
    auto ret = new Image();
    ret->autorelease();

    ret->m_impl->m_width = width;
    ret->m_impl->m_height = height;
    ret->m_impl->m_data = std::move(data);

    return ret;
}

unsigned int Image::getWidth() const {
    return m_impl->m_width;
}

unsigned int Image::getHeight() const {
    return m_impl->m_height;
}

geode::Result<> Image::saveToFile(const std::filesystem::path& path) {
    auto file = std::fopen(string::pathToString(path).c_str(), "wb");
    if (!file) {
        return geode::Err("Couldn't open file: {}", string::pathToString(path).c_str());
    }

    auto context = spng_ctx_new(SPNG_CTX_ENCODER);
    if (!context) {
        std::fclose(file);
        return geode::Err("Couldn't create spng context");
    }

    spng_set_png_file(context, file);

    spng_ihdr ihdr{};
    ihdr.width = m_impl->m_width;
    ihdr.height = m_impl->m_height;
    ihdr.bit_depth = 8;
    ihdr.color_type = SPNG_COLOR_TYPE_TRUECOLOR_ALPHA;
    ihdr.compression_method = 0;
    ihdr.filter_method = 0;
    ihdr.interlace_method = SPNG_INTERLACE_NONE;

    auto result = spng_set_ihdr(context, &ihdr);

    if (result == 0) {
        result = spng_encode_image(
            context,
            m_impl->m_data.data(),
            m_impl->m_data.size() * sizeof(ccColor4B),
            SPNG_FMT_PNG,
            SPNG_ENCODE_FINALIZE
        );
    }

    spng_ctx_free(context);
    std::fclose(file);

    if (result != 0) {
        return geode::Err("Couldn't create image: {}", result);
    }

    return geode::Ok();
}

std::span<cocos2d::ccColor4B> Image::getPixels() {
    return m_impl->m_data;
}

std::span<std::byte> Image::bytes() {
    return std::as_writable_bytes(getPixels());
}
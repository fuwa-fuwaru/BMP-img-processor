#pragma once

#include "image.h"
#include <string>

struct BaseFilter {
    virtual std::string GetType() = 0;
    virtual void Implement(Image& image) = 0;
};

struct Grayscale: public BaseFilter {
    std::string GetType() override;
    void Implement(Image& image) override;
};

struct Negative: public BaseFilter {
    std::string GetType() override;
    void Implement(Image& image) override;
};

struct Crop: public BaseFilter {
    int h_;
    int w_;
    std::string GetType() override;
    void Implement(Image& image) override;
};

struct Sharpening: public BaseFilter {
    std::string GetType() override;
    void Implement(Image& image) override;
};

struct EdgeDetection: public BaseFilter {
    int threshold_;
    std::string GetType() override;
    void Implement(Image& image) override;
};
void BlackWhite(Color& pixel, const uint8_t& value, const int& threshold);

struct ColorClamp: public BaseFilter {
    uint8_t r_min_;
    uint8_t g_min_;
    uint8_t b_min_;
    uint8_t r_max_;
    uint8_t g_max_;
    uint8_t b_max_;
    std::string GetType() override;
    void Implement(Image& image) override;
};
//void BlackWhite(Color& pixel, const uint8_t& value, const int& threshold);
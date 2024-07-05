#include "filters.h"
#include "image.h"

#include <iostream>

std::string Grayscale::GetType() {
    return "Grayscale";
}
void Grayscale::Implement(Image& image) {
    for (auto& row : image.colors_) {
        for (auto& pixel : row) {
            uint8_t gray = 0.299 * pixel.r + 0.587 * pixel.g + 0.114 * pixel.b;
            pixel.r = gray;
            pixel.g = gray;
            pixel.b = gray;
        }
    }
    std::cout << "Implement filter: " << this->GetType() << std::endl;
}

std::string Negative::GetType() {
    return "Negative";
}
void Negative::Implement(Image& image) {
    for (auto& row : image.colors_) {
        for (auto& pixel : row) {
            pixel.r = 255 - pixel.r;
            pixel.g = 255 - pixel.g;
            pixel.b = 255 - pixel.b;
        }
    }
    std::cout << "Implement filter: " << this->GetType() << std::endl;
}

std::string Crop::GetType() {
    return "Crop";
}
void Crop::Implement(Image& image) {
    if (h_ < image.height_) {
        std::move(image.colors_.begin() + image.height_ - h_, image.colors_.begin() + image.height_,
                  image.colors_.begin());
        image.height_ = h_;
    }
    if (w_ < image.width_) {
        image.size_ = (image.size_ / image.width_) * w_;
        image.width_ = w_;
        image.padding_ = (4 - image.size_ % 4) % 4;
    }
    std::cout << "Implement filter: " << this->GetType() << " with width " << w_ << ", height " << h_ <<std::endl;
}

std::string Sharpening::GetType() {
    return "Sharpening";
}
void Sharpening::Implement(Image& image) {
    std::vector<std::vector<Color>> colors_new(image.height_, std::vector<Color>(image.width_));
    for (size_t i = 1; i < image.height_ - 1; ++i) {
        for (size_t j = 1; j < image.width_ - 1; ++j) {
            // WITHOUT BORDERS
            colors_new[i][j].r = std::min(255, std::max(0, - 1 * image.colors_[i - 1][j].r -
                                                               1 * image.colors_[i][j - 1].r +
                                                               5 * image.colors_[i][j].r -
                                                               1 * image.colors_[i][j + 1].r -
                                                               1 * image.colors_[i + 1][j].r));
            colors_new[i][j].g = std::min(255, std::max(0, - 1 * image.colors_[i - 1][j].g -
                                                               1 * image.colors_[i][j - 1].g +
                                                               5 * image.colors_[i][j].g -
                                                               1 * image.colors_[i][j + 1].g -
                                                               1 * image.colors_[i + 1][j].g));
            colors_new[i][j].b = std::min(255, std::max(0, - 1 * image.colors_[i - 1][j].b -
                                                               1 * image.colors_[i][j - 1].b +
                                                               5 * image.colors_[i][j].b -
                                                               1 * image.colors_[i][j + 1].b -
                                                               1 * image.colors_[i + 1][j].b));
          }
    }
    // TOP AND BOTTOM ROWS (NO CORNERS)
    int h = image.height_ - 1;
    int w = image.width_ - 1;
    for (size_t j = 1; j < image.width_ - 1; ++j) {
        // TOP
        colors_new[0][j].r = std::min(255, std::max(0, - 1 * image.colors_[0][j - 1].r + 4 * image.colors_[0][j].r -
                                                           1 * image.colors_[0][j + 1].r - 1 * image.colors_[1][j].r));
        colors_new[0][j].g = std::min(255, std::max(0, - 1 * image.colors_[0][j - 1].g + 4 * image.colors_[0][j].g -
                                                           1 * image.colors_[0][j + 1].g - 1 * image.colors_[1][j].g));
        colors_new[0][j].b = std::min(255, std::max(0, - 1 * image.colors_[0][j - 1].b + 4 * image.colors_[0][j].b -
                                                           1 * image.colors_[0][j + 1].b - 1 * image.colors_[1][j].b));
        // BOTTOM
        colors_new[h][j].r = std::min(255, std::max(0, - 1 * image.colors_[h - 1][j].r - 1 * image.colors_[h][j - 1].r +
                                                           4 * image.colors_[h][j].r - 1 * image.colors_[h][j + 1].r));
        colors_new[h][j].g = std::min(255, std::max(0, - 1 * image.colors_[h - 1][j].g - 1 * image.colors_[h][j - 1].g +
                                                           4 * image.colors_[h][j].g - 1 * image.colors_[h][j + 1].g));
        colors_new[h][j].b = std::min(255, std::max(0, - 1 * image.colors_[h - 1][j].b - 1 * image.colors_[h][j - 1].b +
                                                           4 * image.colors_[h][j].b - 1 * image.colors_[h][j + 1].b));
    }
    // LEFT AND RIGHT COLUMNS (NO CORNERS)
    for (size_t i = 1; i < image.height_ - 1; ++i) {
        // LEFT
        colors_new[i][0].r = std::min(255, std::max(0, - 1 * image.colors_[i - 1][0].r + 4 * image.colors_[i][0].r -
                                                           1 * image.colors_[i][1].r - 1 * image.colors_[i + 1][0].r));
        colors_new[i][0].g = std::min(255, std::max(0, - 1 * image.colors_[i - 1][0].g + 4 * image.colors_[i][0].g -
                                                           1 * image.colors_[i][1].g - 1 * image.colors_[i + 1][0].g));
        colors_new[i][0].b = std::min(255, std::max(0, - 1 * image.colors_[i - 1][0].b + 4 * image.colors_[i][0].b -
                                                           1 * image.colors_[i][1].b - 1 * image.colors_[i + 1][0].b));
        // RIGHT
        colors_new[i][w].r = std::min(255, std::max(0, - 1 * image.colors_[i - 1][w].r - 1 * image.colors_[i][w - 1].r +
                                                           4 * image.colors_[i][w].r - 1 * image.colors_[i + 1][w].r));
        colors_new[i][w].g = std::min(255, std::max(0, - 1 * image.colors_[i - 1][w].g - 1 * image.colors_[i][w - 1].g +
                                                           4 * image.colors_[i][w].g - 1 * image.colors_[i + 1][w].g));
        colors_new[i][w].b = std::min(255, std::max(0, - 1 * image.colors_[i - 1][w].b - 1 * image.colors_[i][w - 1].b +
                                                           4 * image.colors_[i][w].b - 1 * image.colors_[i + 1][w].b));
    }
    // CORNERS
    colors_new[0][0].r = std::min(255, std::max(0, 3 * image.colors_[0][0].r - 1 * image.colors_[0][1].r -
                                                       1 * image.colors_[1][0].r));
    colors_new[0][0].g = std::min(255, std::max(0, 3 * image.colors_[0][0].g - 1 * image.colors_[0][1].g -
                                                       1 * image.colors_[1][0].g));
    colors_new[0][0].b = std::min(255, std::max(0, 3 * image.colors_[0][0].b - 1 * image.colors_[0][1].b -
                                                       1 * image.colors_[1][0].b));

    colors_new[0][w].r = std::min(255, std::max(0, - 1 * image.colors_[0][w - 1].r + 3 * image.colors_[0][w].r -
                                                       1 * image.colors_[1][w].r));
    colors_new[0][w].g = std::min(255, std::max(0, - 1 * image.colors_[0][w - 1].g + 3 * image.colors_[0][w].g -
                                                       1 * image.colors_[1][w].g));
    colors_new[0][w].b = std::min(255, std::max(0, - 1 * image.colors_[0][w - 1].b + 3 * image.colors_[0][w].b -
                                                       1 * image.colors_[1][w].b));

    colors_new[h][w].r = std::min(255, std::max(0, - 1 * image.colors_[h - 1][w].r - 1 * image.colors_[h][w - 1].r +
                                                       3 * image.colors_[h][w].r));
    colors_new[h][w].g = std::min(255, std::max(0, - 1 * image.colors_[h - 1][w].g - 1 * image.colors_[h][w - 1].g +
                                                       3 * image.colors_[h][w].g));
    colors_new[h][w].b = std::min(255, std::max(0, - 1 * image.colors_[h - 1][w].b - 1 * image.colors_[h][w - 1].b +
                                                       3 * image.colors_[h][w].b));

    colors_new[h][0].r = std::min(255, std::max(0, - 1 * image.colors_[h - 1][0].r + 3 * image.colors_[h][0].r -
                                                       1 * image.colors_[h][1].r));
    colors_new[h][0].g = std::min(255, std::max(0, - 1 * image.colors_[h - 1][0].g + 3 * image.colors_[h][0].g -
                                                       1 * image.colors_[h][1].g));
    colors_new[h][0].b = std::min(255, std::max(0, - 1 * image.colors_[h - 1][0].b + 3 * image.colors_[h][0].b -
                                                       1 * image.colors_[h][1].b));

    image.colors_ = colors_new;
    std::cout << "Implement filter: " << this->GetType() <<std::endl;
}

std::string EdgeDetection::GetType() {
    return "Edge Detection";
}
void EdgeDetection::Implement(Image& image) {
    std::vector<std::vector<Color>> colors_new(image.height_, std::vector<Color>(image.width_));
    for (size_t i = 1; i < image.height_ - 1; ++i) {
        for (size_t j = 1; j < image.width_ - 1; ++j) {
            // WITHOUT BORDERS
            uint8_t value = std::min(255, std::max(0, - 1 * image.colors_[i - 1][j].r - 1 * image.colors_[i][j - 1].r +
                                                          4 * image.colors_[i][j].r - 1 * image.colors_[i][j + 1].r -
                                                          1 * image.colors_[i + 1][j].r));
            BlackWhite(colors_new[i][j], value, threshold_);
        }
    }
    // TOP AND BOTTOM ROWS (NO CORNERS)
    int h = image.height_ - 1;
    int w = image.width_ - 1;
    for (size_t j = 1; j < image.width_ - 1; ++j) {
        // TOP
        uint8_t value = std::min(255, std::max(0, - 1 * image.colors_[0][j - 1].r + 3 * image.colors_[0][j].r -
                                                      1 * image.colors_[0][j + 1].r - 1 * image.colors_[1][j].r));
        BlackWhite(colors_new[0][j], value, threshold_);
        // BOTTOM
        value = std::min(255, std::max(0, - 1 * image.colors_[h - 1][j].r - 1 * image.colors_[h][j - 1].r +
                                              3 * image.colors_[h][j].r - 1 * image.colors_[h][j + 1].r));
        BlackWhite(colors_new[h][j], value, threshold_);
    }
    // LEFT AND RIGHT COLUMNS (NO CORNERS)
    for (size_t i = 1; i < image.height_ - 1; ++i) {
        // LEFT
        uint8_t value = std::min(255, std::max(0, - 1 * image.colors_[i - 1][0].r + 3 * image.colors_[i][0].r -
                                                      1 * image.colors_[i][1].r - 1 * image.colors_[i + 1][0].r));
        BlackWhite(colors_new[i][0], value, threshold_);
        // RIGHT
        value = std::min(255, std::max(0, - 1 * image.colors_[i - 1][w].r - 1 * image.colors_[i][w - 1].r +
                                              3 * image.colors_[i][w].r - 1 * image.colors_[i + 1][w].r));
        BlackWhite(colors_new[i][w], value, threshold_);
    }
    // CORNERS
    uint8_t value = std::min(255, std::max(0, 2 * image.colors_[0][0].r - 1 * image.colors_[0][1].r -
                                                  1 * image.colors_[1][0].r));
    BlackWhite(colors_new[0][0], value, threshold_);

    value = std::min(255, std::max(0, - 1 * image.colors_[0][w - 1].r + 2 * image.colors_[0][w].r -
                                          1 * image.colors_[1][w].r));
    BlackWhite(colors_new[0][w], value, threshold_);

    value = std::min(255, std::max(0, - 1 * image.colors_[h - 1][w].r - 1 * image.colors_[h][w - 1].r +
                                          2 * image.colors_[h][w].r));
    BlackWhite(colors_new[h][w], value, threshold_);

    value = std::min(255, std::max(0, - 1 * image.colors_[h - 1][0].r + 2 * image.colors_[h][0].r -
                                          1 * image.colors_[h][1].r));
    BlackWhite(colors_new[h][0], value, threshold_);

    image.colors_ = colors_new;
    std::cout << "Implement filter: " << this->GetType() << std::endl;
}
void BlackWhite(Color& pixel, const uint8_t& value, const int& threshold) {
    if (value > threshold) {
        pixel.r = 255;
        pixel.g = 255;
        pixel.b = 255;
    } else {
        pixel.r = 0;
        pixel.g = 0;
        pixel.b = 0;
    }
}

std::string ColorClamp::GetType() {
    return "Color Clamp";
}
void ColorClamp::Implement(Image& image) {
    for (auto& row : image.colors_) {
        for (auto& pixel : row) {
            if (pixel.r < r_min_) {
                pixel.r = r_min_;
            }
            if (pixel.r > r_max_) {
                pixel.r = r_max_;
            }
            if (pixel.g < g_min_) {
                pixel.g = g_min_;
            }
            if (pixel.g > g_max_) {
                pixel.g = g_max_;
            }
            if (pixel.b < b_min_) {
                pixel.b = b_min_;
            }
            if (pixel.b > b_max_) {
                pixel.b = b_max_;
            }
        }
    }
    std::cout << "Implement filter: " << this->GetType() << std::endl;
}
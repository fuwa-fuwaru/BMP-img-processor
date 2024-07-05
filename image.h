#pragma once

#include <fstream>
#include <string>
#include <vector>

struct Color {
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
};

const size_t BITS_PER_BYTE = 8;
const size_t BYTES_PER_INT = 4;
const size_t BYTE_MASK = 0xFF;

void WriteByte(std::ostream& out, uint8_t value);

void WriteInt(std::ostream& out, uint32_t value);

void WriteZeros(std::ostream& out, size_t count);

const size_t BMP_SIGNATURE_BYTE_1 = 0x42;
const size_t BMP_SIGNATURE_BYTE_2 = 0x4D;
const size_t HEADER_SIZE = 54;
const size_t INFO_HEADER_SIZE = 40;

void WriteBmpHeader(std::ostream& out, size_t height, size_t width, int padding);

uint HexToInt(char* x, size_t start, size_t finish);

class Image {
public:
    Image() = default;
    int width_;
    int height_;
    int padding_;
    int size_;
    std::vector<std::vector<Color>> colors_;
};

Image ReadImage(std::string& input_file);
void WriteImage(std::string& output_file, Image& image);

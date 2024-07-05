#include "image.h"

#include <iostream>

void WriteByte(std::ostream& out, uint8_t value) {
    out.write(reinterpret_cast<char*>(&value), 1);
}

void WriteInt(std::ostream& out, uint32_t value) {
    for (size_t i = 0; i < BYTES_PER_INT; ++i) {
        WriteByte(out, (value >> (i * BITS_PER_BYTE)) & BYTE_MASK);
    }
}

void WriteZeros(std::ostream& out, size_t count) {
    for (size_t i = 0; i < count; ++i) {
        WriteByte(out, 0);
    }
}

void WriteBmpHeader(std::ostream& out, size_t height, size_t width, int padding) {
    WriteByte(out, BMP_SIGNATURE_BYTE_1);
    WriteByte(out, BMP_SIGNATURE_BYTE_2);
    auto file_size =  HEADER_SIZE + height * (width * 3 + padding);
    WriteInt(out, file_size);
    WriteZeros(out, 4);
    WriteInt(out, HEADER_SIZE);
    WriteInt(out, INFO_HEADER_SIZE);
    WriteInt(out, width);
    WriteInt(out, height);
    WriteByte(out, 1);
    WriteZeros(out, 1);
    WriteByte(out, 24);
    WriteZeros(out, 1);
    WriteZeros(out, 24);
}

uint HexToInt(char* x, size_t start, size_t finish) {
    uint tmp = 0;
    for (size_t i = finish - 1; i >= start; --i) {
        tmp <<= 8;
        tmp += x[i];
    }
    return tmp;
}

Image ReadImage(std::string& input_file) {
    Image image;
    std::ifstream in(input_file, std::ios::binary);
    char header[54];
    in.read(header, 54);
    int width = HexToInt(header, 18, 22);
    int height = HexToInt(header, 22, 26);
    int data_offset = HexToInt(header, 10, 14);
    int bit_count = HexToInt(header, 28, 30);
    int byte_count = bit_count / 8;
    std::vector<std::vector<Color>> colors(height, std::vector<Color>(width));
    int real_size = byte_count * width;
    int padding = (4 - real_size % 4) % 4;
    int start = data_offset;
    int finish = start + real_size;
    for (int i = 0; i < height; ++i) {
        in.seekg(start);
        for (int j = start; j < finish;) {
            in.read(reinterpret_cast<char*>(&colors[i][(j - start) / 3].b), 1);
            in.read(reinterpret_cast<char*>(&colors[i][(j - start) / 3].g), 1);
            in.read(reinterpret_cast<char*>(&colors[i][(j - start) / 3].r), 1);
            j += 3;
        }
        start = finish + padding;
        finish = start + real_size;
    }
    image.colors_ = colors;
    image.width_ = width;
    image.height_ = height;
    image.size_ = real_size;
    image.padding_ = padding;
    return image;
}

void WriteImage(std::string& output_file, Image& image) {
    std::ofstream out(output_file, std::ios::binary);
    WriteBmpHeader(out, image.height_, image.width_, image.padding_);
    for (size_t i = 0; i < image.height_; ++i) {
        for (size_t j = 0; j < image.width_; ++j) {
            WriteByte(out, image.colors_[i][j].b);
            WriteByte(out, image.colors_[i][j].g);
            WriteByte(out, image.colors_[i][j].r);
        }
        WriteZeros(out, image.padding_);
    }
    std::cout << "new file: " << output_file << std::endl;
}
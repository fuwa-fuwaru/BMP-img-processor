#include "arg_parser.h"

#include <iostream>
#include <cctype>

void Help() {
    std::cout << "usage: image_processor input_file output_file\n" << std::endl;
    std::cout << "This is a list of available filters:\n" << std::endl;
    std::cout << "\t -gs \t\t grayscale filter" << std::endl;
    std::cout << "\t -neg \t\t negative filter" << std::endl;
    std::cout << "\t -crop width height \t\t crop image" << std::endl;
    std::cout << "\t -sharp \t\t sharpening filter" << std::endl;
    std::cout << "\t -edge threshold \t\t edge detection filter" << std::endl;
    std::cout << "\t -clamp r_min[0, 255] g_min[0, 255] b_min[0, 255] r_max[0, 255] g_max[0, 255] b_max[0, 255] \t\t"
                 "color clamp filter" << std::endl;
}

ProgramArgs Parser::operator()(int argc, char** argv) {
    ProgramArgs args;
    if (argc == 1) {
        Help();
        std::exit(EXIT_SUCCESS);
    } else if (argc < 3) {
        throw std::invalid_argument("Argument error: not enough arguments passed");
    }
    args.file_in = argv[1];
    std::cout << "file in: " << argv[1] << std::endl;
    args.file_out = argv[2];
    std::cout << "file out: " << argv[2] << std::endl;
    int i = 3;
    while (i < argc) {
        if (std::string(argv[i]) == std::string("-gs")) {
            std::cout << "filter: grayscale" << std::endl;
            std::shared_ptr<Grayscale> filter = std::make_shared<Grayscale>();
            args.filters.push_back(filter);
            i += 1;
        } else if (std::string(argv[i]) == std::string("-neg")) {
            std::cout << "filter: negative" << std::endl;
            std::shared_ptr<Negative> filter = std::make_shared<Negative>();
            args.filters.push_back(filter);
            i += 1;
        } else if (std::string(argv[i]) == std::string("-crop")) {
            if (argc - i <= 2 || !std::isdigit(*argv[i + 1]) || !std::isdigit(*argv[i + 2]) ||
                std::stoi(argv[i + 1]) <= 0 || std::stoi(argv[i + 2]) <= 0) {
                throw std::invalid_argument("Crop filter error: invalid size");
            } else {
                std::shared_ptr<Crop> filter = std::make_shared<Crop>();
                filter->w_ = std::stoi(argv[i + 1]);
                filter->h_ = std::stoi(argv[i + 2]);
                args.filters.push_back(filter);
                i += 3;
            }
        } else if (std::string(argv[i]) == std::string("-sharp")) {
            std::cout << "filter: sharpening" << std::endl;
            std::shared_ptr<Sharpening> filter = std::make_shared<Sharpening>();
            args.filters.push_back(filter);
            i += 1;
        } else if (std::string(argv[i]) == std::string("-edge")) {
            if (argc - i <= 1 || !std::isdigit(*argv[i + 1])) {
                throw std::invalid_argument("Edge filter error: invalid threshold");
            } else {
                std::cout << "filter: grayscale (for edge detection)" << std::endl;
                std::shared_ptr<Grayscale> filter_1 = std::make_shared<Grayscale>();
                args.filters.push_back(filter_1);
                std::cout << "filter: edge detection" << std::endl;
                std::shared_ptr<EdgeDetection> filter_2 = std::make_shared<EdgeDetection>();
                filter_2->threshold_ = std::stoi(argv[i + 1]);
                args.filters.push_back(filter_2);
                i += 2;
            }
        } else if (std::string(argv[i]) == std::string("-clamp")) {
            if (argc - i <= 6 || !std::isdigit(*argv[i + 1]) || !std::isdigit(*argv[i + 2]) ||
                !std::isdigit(*argv[i + 3]) || !std::isdigit(*argv[i + 4]) || !std::isdigit(*argv[i + 5]) ||
                !std::isdigit(*argv[i + 6]) || std::stoi(argv[i + 1]) > std::stoi(argv[i + 4]) ||
                std::stoi(argv[i + 2]) > std::stoi(argv[i + 5]) || std::stoi(argv[i + 3]) > std::stoi(argv[i + 6]) ||
                std::stoi(argv[i + 4]) > 255 || std::stoi(argv[i + 5]) > 255 || std::stoi(argv[i + 6]) > 255) {
                throw std::invalid_argument("Clamp filter error: invalid parameters");
            } else {
                std::cout << "filter: color clamp" << std::endl;
                std::shared_ptr<ColorClamp> filter = std::make_shared<ColorClamp>();
                filter->r_min_ = std::stoi(argv[i + 1]);
                filter->g_min_ = std::stoi(argv[i + 2]);
                filter->b_min_ = std::stoi(argv[i + 3]);
                filter->r_max_ = std::stoi(argv[i + 4]);
                filter->g_max_ = std::stoi(argv[i + 5]);
                filter->b_max_ = std::stoi(argv[i + 6]);
                args.filters.push_back(filter);
                i += 7;
            }
        } else {
            throw std::invalid_argument("Argument error: unknown parameter!");
        }
    }
    return args;
}

#include "image.h"
#include "arg_parser.h"
#include "pipeline.h"

#include <iostream>
#include <vector>

int main(int argc, char** argv) {
    Parser parser;
    ProgramArgs args;
    try {
        args = parser(argc, argv);
    } catch (std::exception& e) {
        std::cout << "Parsing error: " << e.what() << std::endl;
        std::exit(EXIT_FAILURE);
    }
    Image image = ReadImage(args.file_in);
    Pipeline(image, args);
    WriteImage(args.file_out, image);
}

#pragma once

#include "filters.h"

#include <string>
#include <vector>
#include <memory>

struct ProgramArgs {
    std::string file_in;
    std::string file_out;
    std::vector<std::shared_ptr<BaseFilter>> filters;
};

class Parser {
public:
    ProgramArgs operator()(int argc, char** argv);
};

void Help();

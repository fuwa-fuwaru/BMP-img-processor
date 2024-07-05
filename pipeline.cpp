#include "pipeline.h"
#include "filters.h"

void Pipeline(Image& image, const ProgramArgs& args) {
    for (auto& f : args.filters) {
        f->Implement(image);
    }
}

cmake_minimum_required(VERSION 3.17)
project(image_processor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

set(Sources arg_parser.cpp, image.cpp, filters.cpp)
set(Headers arg_parser.h, image.h, filters.h)

add_executable(image_processor
    image_processor.cpp
    image.cpp
    arg_parser.cpp
    filters.cpp
    pipeline.cpp
)

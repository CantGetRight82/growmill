cmake_minimum_required(VERSION 2.8.9)
project (growmill)
file(GLOB SOURCES "src/core/*.cpp")

link_directories("/usr/local/lib")
include_directories("/usr/local/include/cairo")
include_directories("src")
add_executable(growc platform/cairo/cairo.cc ${SOURCES})
target_compile_features(growc PRIVATE cxx_range_for)

target_link_libraries(growc "cairo")

#find_path (GD_INCLUDE gd.h)
#find_library(GD_LIBRARY gd)
#if (GD_INCLUDE AND GD_LIBRARY)
#message("found ${GD_INCLUDE}")
#include_directories(${GD_INCLUDE})
#endif (GD_INCLUDE AND GD_LIBRARY)
#target_link_libraries (growc /usr/local/lib/libgd.dylib)


cmake_minimum_required(VERSION 2.8.9)
project (growmill)
file(GLOB SOURCES "src/core/*.cpp")
file(GLOB CSOURCES "platform/cairo/*.cc")
file(GLOB SVG_SOURCES "platform/svg/*.cpp")
file(GLOB INCLUDES "src/core/*.h")

link_directories("/usr/local/lib")
include_directories("/usr/local/include")
include_directories("/usr/local/include/freetype2")
include_directories("/usr/local/include/cairo")
include_directories("src")
add_executable(growc ${CSOURCES} ${SOURCES} ${INCLUDES})
target_compile_features(growc PRIVATE cxx_range_for)

target_link_libraries(growc "cairo")
target_link_libraries(growc "freetype")

add_executable(yy ${SVG_SOURCES} ${SOURCES} ${INCLUDES})
target_compile_features(yy PRIVATE cxx_range_for)
target_link_libraries(yy "freetype")


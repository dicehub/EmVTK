cmake_minimum_required(VERSION 3.7)

project(emvtk)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL=2 \
    -s USE_WEBGL2=1 -s FULL_ES2=1 -std=c++11 --bind \
    -s ERROR_ON_UNDEFINED_SYMBOLS=0 -s ALLOW_MEMORY_GROWTH=1 -g3 \
    -s DEMANGLE_SUPPORT=1 -s WASM=1 -s EMULATE_FUNCTION_POINTER_CASTS=0 \
    -s MODULARIZE=1 -s EXPORT_NAME=emvtk -s USE_FREETYPE=1 -O2 -s FORCE_FILESYSTEM=1 \
    -s EXTRA_EXPORTED_RUNTIME_METHODS=[\"FS\"]")

file(GLOB emvtk_SRC
    "${CMAKE_SOURCE_DIR}/src/*.h"
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

add_executable(emvtk ${emvtk_SRC})

add_custom_command(TARGET emvtk POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/index.html"
        $<TARGET_FILE_DIR:emvtk>)

find_package(VTK REQUIRED NO_MODULE)
include(${VTK_USE_FILE})
target_link_libraries(emvtk ${VTK_LIBRARIES})


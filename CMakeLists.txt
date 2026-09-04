cmake_minimum_required(VERSION 3.20)
project(scratch LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Prebuilt SDL2 (mingw devel package) bundled in third_party/.
# Point CMake at the arch-specific tree so find_package(SDL2) picks up
# the SDL2Config.cmake shipped inside it.
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(SDL2_ARCH_DIR "${CMAKE_SOURCE_DIR}/third_party/SDL2/x86_64-w64-mingw32")
else()
    set(SDL2_ARCH_DIR "${CMAKE_SOURCE_DIR}/third_party/SDL2/i686-w64-mingw32")
endif()
list(APPEND CMAKE_PREFIX_PATH "${SDL2_ARCH_DIR}")

find_package(SDL2 REQUIRED CONFIG)

add_executable(scratch 
src/main.cpp
src/Window.cpp
src/Window.h
src/Application.h
src/Application.cpp
src/Structures.h
src/Structures.cpp
src/Mesh.h
src/Mesh.cpp
src/Triangle.h
src/Triangle.cpp
src/Draw.h
src/Draw.cpp
src/Buffer.h
src/Buffer.cpp
src/Matrix.h
src/Matrix.cpp
src/Light.h
src/Light.cpp
)

target_compile_definitions(scratch PRIVATE ASSETS_DIR="${CMAKE_SOURCE_DIR}/assets/")

target_link_libraries(scratch PRIVATE SDL2::SDL2main SDL2::SDL2)

# Drop SDL2.dll next to the built executable so it runs without
# needing SDL2 on PATH.
add_custom_command(TARGET scratch POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SDL2_ARCH_DIR}/bin/SDL2.dll"
        "$<TARGET_FILE_DIR:scratch>"
)


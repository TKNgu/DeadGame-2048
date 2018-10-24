# Cmake version
cmake_minimum_required(VERSION 3.10)

# C++ std 14
set(CMAKE_CXX_STANDARD 14)

# Project DeadGame-2048
set(PROJECT DeadGame-2048)

# Code file.
file(GLOB SOURCES "src/*.cpp" "src/*.c")

# Header file.
include_directories(${CMAKE_SOURCE_DIR}/usr/include)
include_directories(include)
include_directories(lib)

# External lib.
link_directories(${CMAKE_SOURCE_DIR}/usr/lib)

# Build exec file.
project(${PROJECT})
add_executable(${PROJECT} ${SOURCES})
target_link_libraries(${PROJECT} sfml-system sfml-graphics sfml-window appimageupdate)

# Build AppImage.
add_custom_target(AppImage)
add_dependencies(AppImage ${PROJECT})
add_custom_command(TARGET AppImage
                     COMMAND ${CMAKE_COMMAND} -E
                         copy_directory ${CMAKE_SOURCE_DIR}/DeadGame-2048 AppImage)
add_custom_command(TARGET AppImage
                     COMMAND ${CMAKE_COMMAND} -E
                         copy ${PROJECT} AppImage/bin/${PROJECT})
add_custom_command(TARGET AppImage
                     COMMAND ${CMAKE_COMMAND} -E
                         copy_directory ${CMAKE_SOURCE_DIR}/data AppImage/data)
add_custom_command(TARGET AppImage
                     COMMAND ${CMAKE_COMMAND} -E
                         copy_directory ${CMAKE_SOURCE_DIR}/usr/lib AppImage/lib)
add_custom_command(TARGET AppImage
                     COMMAND ${CMAKE_SOURCE_DIR}/tool/appimagetool-x86_64.AppImage AppImage/)
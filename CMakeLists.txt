# Cmake version
cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 14)

set(PROJECT DeadGame-2048)
set(LIB_SFML SFML)

file(GLOB SOURCES "src/*.cpp" "src/*.c")
add_subdirectory(${LIB_SFML})

include_directories(include)
include_directories(lib)

include_directories(/home/ngocpt/Work/DeadGame-2048/SFML/include)
include_directories(SFML/lib)

project(${PROJECT})
add_executable(${PROJECT} ${SOURCES})
target_link_libraries(${PROJECT} sfml-graphics sfml-window)

add_custom_command(TARGET AppImage
                     COMMAND ${CMAKE_COMMAND} -E
                         copy_directory ${CMAKE_SOURCE_DIR}/AppImage AppImage)
add_custom_command(TARGET AppImage
                     COMMAND ${CMAKE_COMMAND} -E
                         copy ${PROJECT} AppImage/bin/${PROJECT})
add_custom_command(TARGET AppImage
                     COMMAND ${CMAKE_COMMAND} -E
                         copy_directory SFML/lib AppImage/lib)
add_custom_command(TARGET AppImage
                     COMMAND ${CMAKE_COMMAND} -E
                         copy_directory ${CMAKE_SOURCE_DIR}/data AppImage/data)
add_custom_command(TARGET AppImage
                     COMMAND ${CMAKE_SOURCE_DIR}/tool/appimagetool-x86_64.AppImage AppImage/)
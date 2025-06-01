cmake_minimum_required(VERSION 3.16)
project(ascii)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#SET(CMAKE_CXX_COMPILER "/usr/local/bin/g++-14" CACHE STRING "C++ compiler" FORCE)

find_package(Curses REQUIRED)

add_compile_options()

add_executable(
    ascii
    main.cc
    image.cc
)
target_include_directories(ascii PRIVATE ${CURSES_INCLUDE_DIRS})
target_link_libraries(ascii ${CURSES_LIBRARIES})
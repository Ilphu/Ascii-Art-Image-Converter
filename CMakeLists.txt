cmake_minimum_required(VERSION 3.12)
project(ascii_project)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(OpenCV REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(NCURSES REQUIRED ncurses)

# Add executable
add_executable(ascii main.cc image.cc)

# Include directories
target_include_directories(ascii PRIVATE ${OpenCV_INCLUDE_DIRS})
target_include_directories(ascii PRIVATE ${NCURSES_INCLUDE_DIRS})

# Link libraries
target_link_libraries(ascii ${OpenCV_LIBS})
target_link_libraries(ascii ${NCURSES_LIBRARIES})

# Compiler flags for NCurses
target_compile_options(ascii PRIVATE ${NCURSES_CFLAGS_OTHER})

# Optional: Set compiler flags for debugging/optimization
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(ascii PRIVATE -g -O0 -Wall -Wextra)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(ascii PRIVATE -O3 -DNDEBUG)
endif()

# Print some information
message(STATUS "OpenCV version: ${OpenCV_VERSION}")
message(STATUS "OpenCV libraries: ${OpenCV_LIBS}")
message(STATUS "NCurses libraries: ${NCURSES_LIBRARIES}")
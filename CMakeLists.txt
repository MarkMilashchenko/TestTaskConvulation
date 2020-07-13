cmake_minimum_required(VERSION 3.5)

add_compile_options(
    -Werror
    -Wall
    -Wextra
    -Wpedantic
    -lfftw3
    -lm
)

project(TestTask)

#SET(CMAKE_C_COMPILER gcc)
#SET(CMAKE_CXX_COMPILER g++)
#file(GLOB SOURCES "*.cpp")
SET(CMAKE_CXX_FLAGS "-std=c++11")
SET(CMAKE_C_FLAGS   "")
#SET(CMAKE_CXX_FLAGS "-lm -lfftw3")
#SET(CMAKE_C_FLAGS   "-lm -lfftw3")
INCLUDE_DIRECTORIES(/usr/include)

set(SOURCE_EXE main.cpp)

set(SOURCE_LIB filter.cpp)

add_library(filter STATIC ${SOURCE_LIB})

include_directories(fftw)

add_executable(start ${SOURCE_EXE})

target_link_libraries(start filter fftw3)

add_subdirectory(fftw)

cmake_minimum_required(VERSION 3.23)
project(hex)

set(CMAKE_CXX_STANDARD 17)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(hex main.cpp)

target_link_libraries(hex ${CURSES_LIBRARIES})







cmake_minimum_required(VERSION 3.5)
project(juego)

set(CMAKE_CXX_STANDARD 11)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(juego juego.cpp)
target_link_libraries(juego sfml-graphics sfml-window sfml-system)


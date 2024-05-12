cmake_minimum_required(VERSION 2.8)
project(game)

set(CMAKE_CXX_STANDARD 17)

add_executable(game main.cpp TTTGame.cpp utils.cpp)
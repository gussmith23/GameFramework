cmake_minimum_required(VERSION 3.1)
project(GameFrameworkParser)

add_executable(parser main.cpp)
set_target_properties(parser PROPERTIES CXX_STANDARD 11 CXX_STANDARD_REQUIRED ON)

cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)

file(GLOB imguiFiles *.h *.cpp)
add_library(imguiLib ${imguiFiles})
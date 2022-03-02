cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)

include_directories(glfw-3.3.6/include)
add_subdirectory(glfw-3.3.6)

include_directories(imgui)
add_subdirectory(imgui)

add_subdirectory(glm)

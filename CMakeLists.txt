cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
project(thesis)

find_package(OpenGL REQUIRED)
include_directories(3rdParty)
add_subdirectory(3rdParty)

file(GLOB projectFiles src/*.h src/*.cpp)

add_executable(thesis ${projectFiles})
target_link_libraries(thesis glfw OpenGL::GL imguiLib)

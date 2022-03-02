#pragma once

#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include "glm/ext/scalar_constants.hpp"

#include <chrono>

using mat4 = glm::mat4x4;
using vec4 = glm::vec4;
constexpr float pi = glm::pi<float>();

using TimePoint = std::chrono::high_resolution_clock::time_point;


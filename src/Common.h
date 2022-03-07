#pragma once

#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <chrono>
#include <iostream>
#include <string>

using mat4 = glm::mat4x4;
using vec4 = glm::vec4;
using vec3 = glm::vec3;
constexpr float pi = glm::pi<float>();

using TimePoint = std::chrono::high_resolution_clock::time_point;

class Ray {
public:
    vec3 mPos;
    vec3 mDir;
};

struct pixel {
    uint8_t r, g, b, a;
};

#if ENABLE_TRACE
class FunctionTracer {
public:
    explicit FunctionTracer(const char* name) :
            mName{name} {
        std::cout << "Start " << mName << '\n';
    }

    ~FunctionTracer() {
        std::cout << "End   " << mName << '\n';
    }

    std::string mName;
};
#define TRACEME FunctionTracer{__func__};
#else
#define TRACEME ((void)0);
#endif
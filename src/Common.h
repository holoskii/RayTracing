#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <chrono>
#include <iostream>
#include <string>


using vec2 = glm::vec2;
using vec3 = glm::vec3;
using vec4 = glm::vec4;
using mat4 = glm::mat4x4;
using TimePoint = std::chrono::high_resolution_clock::time_point;

constexpr float pi = glm::pi<float>();
constexpr float pi2 = glm::pi<float>() * 2.0f;
constexpr float piDiv2 = glm::pi<float>() / 2.0f;

inline float degToRad(float deg) {
    return deg * pi / 180.0f;
}

inline float radToDeg(float rad) {
    return rad / pi * 180.0f;
}

class Ray {
public:
    vec3 mPos;
    vec3 mDir;
};

class Pixel {
public:
    Pixel() = default;

    Pixel(uint8_t _r, uint8_t _g, uint8_t _b) :
        r(_r), g(_g), b(_b) { a = 255; }

private:
    uint8_t r, g, b;
    uint8_t a;
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
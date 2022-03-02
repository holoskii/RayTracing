#pragma once

#include "Common.h"
#include <glm/vec4.hpp>
#include <string>
#include <vector>

#define USE_TIMERS  true

enum class RenderMode { Fast = 1, Slow = 2, Advanced = 3 };
const std::vector<std::string> RenderModeStringVector = { "", "Fast", "Slow", "Advanced" };
static std::string RenderModeString(RenderMode renderMode) {
    return RenderModeStringVector.at((uint64_t)renderMode);
}

class Config {
public:
    const char      defaultFontLocation[128]    = "../../3rdParty/imgui/misc/fonts/DroidSans.ttf";

    float           fontSize                    = 16.0f;
    float           fontScale                   = 2.0f;

    RenderMode      renderMode                  = RenderMode::Fast;


    vec4            backGroundColor             = { 0.45f, 0.55f, 0.60f, 1.00f };
    uint32_t        windowWidth                 = 1920;
    uint32_t        windowHeight                = 1080;
    uint32_t        renderWidth                 = 1920;
    uint32_t        renderHeight                = 1080;
    uint32_t        frameUpdateTime             = 30;
    uint32_t        threadsCount                = 30;
    uint32_t        tileSize                    = 128;
};
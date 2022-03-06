#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <string>
#include <vector>

#define ENABLE_TIMERS   true
#define ENABLE_TRACE    false

enum class RenderMode { Fast = 1, Slow = 2, Advanced = 3 };
const std::vector<std::string> RenderModeStringVector = { "", "Fast", "Slow", "Advanced" };
static std::string RenderModeString(RenderMode renderMode) {
    return RenderModeStringVector.at((uint64_t)renderMode);
}

class Config {
public:
    const char  fontPath[128]    = "../../3rdParty/imgui/misc/fonts/DroidSans.ttf";
    float       fontSize        = 16.0f;
    float       fontScale       = 2.0f;
    RenderMode  renderMode      = RenderMode::Advanced;
    glm::vec4   backGroundColor = { 0.45f, 0.55f, 0.60f, 1.00f };
    uint32_t    windowWidth     = 1920;
    uint32_t    windowHeight    = 1080;
    uint32_t    renderWidth     = 1920;
    uint32_t    renderHeight    = 1080;
    uint32_t    frameUpdateTime = 30;
    uint32_t    threadsCount    = 30;
    uint32_t    tileSize        = 128;
    float       fov             = glm::pi<float>() / 2;
    glm::vec3   cameraPosition  = { 0, 0, 0 };
    glm::vec3   cameraRotation  = { 0, 0, 0 };
};
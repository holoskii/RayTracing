#pragma once

#include "Common.h"
#include <array>
#include <string_view>

#define ENABLE_TIMERS   true
#define ENABLE_TRACE    false

enum class RenderMode { Fast = 1, Slow = 2, Advanced = 3 };
static constexpr std::array<std::string_view, 4> getRenderModes() {
    return { "", "Fast", "Slow", "Advanced" };
}


class Config {
public:
    /// GUI, Application
    const char  fontPath[128]    = "../../3rdParty/imgui/misc/fonts/DroidSans.ttf";
    float       fontSize        = 16.0f;
    float       fontScale       = 2.0f;
    glm::vec4   appBGColor      = { 0.45f, 0.55f, 0.60f, 1.00f };
    uint32_t    windowWidth     = 1920;
    uint32_t    windowHeight    = 1080;

    /// Render
    RenderMode  renderMode      = RenderMode::Advanced;
    float       renderScale     = 0.25f;
    uint32_t    renderWidth     = 1080 * renderScale;
    uint32_t    renderHeight    = 1080 * renderScale;
    Pixel       renderBGColor   = { 74, 75, 82 };
    uint32_t    frameUpdateTime = 30;
    uint32_t    threadsCount    = 1;
    uint32_t    tileSize        = 128;
    float       photonRadius    = 0.05f;  // Radius, at which photons are used. Defines grid size

    /// Camera
    float       fov             = pi / 4;
    glm::vec3   cameraPosition  = { 0.5, 0.5, 0.75 };
    glm::vec3   cameraRotation  = { 0, 0, 0 };

    /// Debug
    int debugX                  = 102;
    int debugY                  = 44;
    bool showDebugPixel         = false;
    bool debugPixel             = false;
};
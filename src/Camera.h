#pragma once

#include "Common.h"
#include "Config.h"

/// Direction, FOV (from config)
/// Used to get camera rays
/// Move and turn calculated here
class Camera {
public:
    float fov = pi / 2; // 90 deg FOV
    vec3 rot; // rotation, only y and x is used
    vec3 pos = {0,0,0};

    vec3 lookDir = glm::normalize(vec3{0, 0, -1});
    vec3 rightVec = glm::normalize(glm::cross(lookDir, {0, 1, 0}));
    vec3 upVec = glm::normalize(glm::cross(rightVec, lookDir));

    /// Screen space coordinates (1, 1) is upper right
    Ray getRay(Config &config, float x, float y);
};
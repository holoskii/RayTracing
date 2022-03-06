#pragma once

#include "Common.h"
#include "Config.h"

/// Coordinates
/// y
/// ▲    -z
/// │  /
/// │ /
/// └───► x
/// Rotation
/// Right-hand rule
/// FIXME https://learnopengl.com/Getting-started/Camera
/// FIXME https://www.scratchapixel.com/code.php?id=7&origin=/lessons/3d-basic-rendering/ray-tracing-generating-camera-rays&src=0

/// Only function - get camera rays
class Camera {
public:
    Camera(Config& config);

    /// Screen space coordinates (-1 ... 1)
    Ray getRay(Config &config, float x, float y);

    /// Updates position vectors(forward, up, right)
    void setRotation(const vec3& rot);
    void setPosition(const vec3& pos);

private:
    /// Camera parameters
    Config& mConfig;
    float   mFov;
    vec3    mPos;

    /// Stores rotation info
    vec3    mLookDir;
    vec3    mRightVec;
    vec3    mUpVec;
};
#include "Camera.h"

Camera::Camera(Config& config)
    : mConfig(config)
    , mFov(mConfig.fov)
    , mPos(mConfig.cameraPosition) {
    setRotation(mConfig.cameraRotation);
}

void Camera::setRotation(const vec3& rot) {
    /// Rotate x first, y second
    mat4 xRotMat = {
            {1, 0, 0, 0},
            {0, cosf(rot.x), -sinf(rot.x), 0},
            {0, sinf(rot.x), cosf(rot.x), 0},
            {0, 0, 0, 1}
    };
    mat4 yRotMat = {
            {cosf(rot.y), 0, -sinf(rot.y), 0},
            {0, 1, 0, 0},
            {sinf(rot.y), 0, cosf(rot.y), 0},
            {0, 0, 0, 1}
    };
    mat4 resultMat = yRotMat * xRotMat;

    vec4 dir = resultMat * vec4{0, 0, -1, 1};
    mLookDir = glm::normalize(vec3{dir.x, dir.y, dir.z});
    mRightVec = glm::normalize(glm::cross(mLookDir, {0, 1, 0}));
    mUpVec = glm::normalize(glm::cross(mRightVec, mLookDir));
}

void Camera::setPosition(const vec3& pos) {
    mPos = pos;
}

Ray Camera::getRay(Config & config, float x, float y) {
    /// FIXME: most of this operation can be moved out
    float width = x * tanf(mFov / 2) * 2;
    float height = y * tanf(mFov / 2) * 2 * (1.0f * config.renderHeight / config.renderWidth);
    vec3 pixelDir = glm::normalize(mLookDir + width * mRightVec + height * mUpVec);
    return { mPos, pixelDir };
}

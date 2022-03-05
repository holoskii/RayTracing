#include "Camera.h"

Ray Camera::getRay(Config & config, float x, float y) {
    float width = x * tanf(fov / 2) * 2;
    float height = y * tanf(fov / 2) * 2 * (1.0f * config.renderHeight / config.renderWidth);
    vec3 pixelDir = glm::normalize(lookDir + width * rightVec + height * upVec);
    return { pos, pixelDir };
}
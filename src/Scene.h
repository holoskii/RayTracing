#pragma once

#include "Objects.h"
#include "Camera.h"
#include "Config.h"
#include "RenderBasics.h"
#include <vector>
#include <memory>


static vec3 dirToCol(vec3 vec) {
    return vec3 {
            vec.x / 2 + 0.5,
            vec.y / 2 + 0.5,
            vec.z / 2 + 0.5
    };
}

static Pixel colorVec3ToPix(vec3 vec) {
    return Pixel {
            (uint8_t)((vec.x) * 255),
            (uint8_t)((vec.y) * 255),
            (uint8_t)((vec.z) * 255)
    };
}

class Scene {


public:
    Scene(Config& config);

    vec3 illuminatePoint(IntersectInfo& intrInfo);
    Pixel renderPixel(uint32_t x, uint32_t y);

    Camera                  mCamera;
    Config&                 mConfig;
    std::vector<std::unique_ptr<Object>>        mObjects;
    std::vector<std::unique_ptr<LightSource>>   mLights;
};
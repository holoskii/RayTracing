#pragma once

#include "Objects.h"
#include "Camera.h"
#include "Config.h"
#include "RenderBasics.h"
#include <vector>
#include <memory>

class Scene {
    static constexpr pixel intrPix{255, 0, 0, 255};
    static constexpr pixel nintrPix{0, 0, 0, 255};

public:
    Scene(Config& config);
    pixel getPixel(uint32_t x, uint32_t y) {
        bool intr = false;
        Ray ray = mCamera.getRay(mConfig, 2.0f * x / mConfig.renderWidth - 1.0f, 2.0f * y / mConfig.renderHeight - 1.0f);
        for(auto& obj : mObjects) {
            intr |= obj->intersect(ray);
        }
        return intr ? intrPix : nintrPix;
    }

private:
    Camera                  mCamera;
    Config&                 mConfig;
    std::vector<std::unique_ptr<Object>>     mObjects;
};
#pragma once

#include "Objects.h"
#include "Camera.h"
#include "Config.h"
#include "RenderBasics.h"
#include <vector>
#include <memory>

static Pixel dirVec3ToPix(vec3& vec) {
    return Pixel {
            (uint8_t)((vec.x / 2 + 0.5) * 255),
            (uint8_t)((vec.y / 2 + 0.5) * 255),
            (uint8_t)((vec.z / 2 + 0.5) * 255)
    };
}

class Scene {


public:
    Scene(Config& config);
    Pixel renderPixel(uint32_t x, uint32_t y) {
        // FIXME: non optimal
        Ray ray = mCamera.getRay(mConfig, 2.0f * x / mConfig.renderWidth - 1.0f, 2.0f * y / mConfig.renderHeight - 1.0f);
        IntersectInfo finalIntr = { false };
        for(auto& obj : mObjects) {
            IntersectInfo intersectInfo = obj->intersect(ray);
            bool replaceFinalIntrWithCurrent = intersectInfo.intersect && (finalIntr.intersect == false || finalIntr.distance > intersectInfo.distance);
            if(replaceFinalIntrWithCurrent) {
                finalIntr = intersectInfo;
            }
        }

        if(!finalIntr.intersect) {
            return mConfig.renderBGColor;
        }

        Object* & obj = finalIntr.object;


        return dirVec3ToPix(finalIntr.normal);
    }

    Camera                  mCamera;
    Config&                 mConfig;
    std::vector<std::unique_ptr<Object>>     mObjects;
};
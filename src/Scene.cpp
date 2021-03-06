#include "Scene.h"


Scene::Scene(Config& config)
    : mConfig(config)
    , mCamera(config) {
    mLights.emplace_back(
        new PointLightSource(
            vec3{ 0.5, 0.99, -0.5 },
            vec3{1, 1, 1},
            0.75,
            "Point light"
        )
    );

    mObjects.emplace_back(
        new Sphere(
            "Smaller",
            vec3{0.3, 0.2, -0.5},
            Material{vec3{1}},
            config,
            0.1
        )
    );

    mObjects.emplace_back(
        new Sphere(
            "Bigger",
            vec3{0.7, 0.3, -0.3},
            Material{vec3{1}},
            config,
            0.15
        )
    );

    // floor
    mObjects.emplace_back(
        new Rectangle(
            "Floor",
            vec3{0,0,0},
            Material{vec3{1}},
            config,
            vec3{1,0,0},
            vec3{0,0,-1}
        )
    );

    // ceiling
    mObjects.emplace_back(
        new Rectangle(
            "Ceiling",
            vec3{0,1,0},
            Material{vec3{1}},
            config,
            vec3{0,0,-1},
            vec3{1,0,0}
        )
    );

    // back
    mObjects.emplace_back(
        new Rectangle(
            "Back",
            vec3{0,0,-1},
            Material{vec3{0, 1, 0}},
            config,
            vec3{1,0,0},
            vec3{0,1,0}
        )
    );

    // left
    mObjects.emplace_back(
        new Rectangle(
            "Left",
            vec3{0,0,0},
            Material{vec3{1, 0, 0}},
            config,
            vec3{0,0,-1},
            vec3{0,1,0}
        )
    );

    // right
    mObjects.emplace_back(
        new Rectangle(
            "Right",
            vec3{1,0,0},
            Material{vec3{0, 0, 1 }},
            config,
            vec3{0,1,0},
            vec3{0,0,-1}
        )
    );
}

vec3 Scene::illuminatePoint(IntersectInfo& intrInfo) {
    bool intersect = false;
    IntersectInfo finalIntr = { false };
    for(auto& light : mLights) {
        Ray ray = { intrInfo.point, glm::normalize(light->mPos - intrInfo.point) };

        for(auto& object : mObjects) {
            IntersectInfo currentIntrInfo;
            bool res = object->intersect(ray, currentIntrInfo);
            if(!res) {
                continue;
            }
            assert(currentIntrInfo.uv.x >=0 && currentIntrInfo.uv.x <= 1);
            assert(currentIntrInfo.uv.y >=0 && currentIntrInfo.uv.y <= 1);
            bool replaceFinalIntrWithCurrent = currentIntrInfo.intersect && (finalIntr.intersect == false || finalIntr.distance > currentIntrInfo.distance);
            if(replaceFinalIntrWithCurrent) {
                finalIntr = currentIntrInfo;
            }
        }
        float dist = glm::distance(light->mPos, intrInfo.point);
        if(finalIntr.intersect && finalIntr.distance < dist) {
            return vec3{0};
        }

        float dp = glm::dot(ray.mDir, intrInfo.normal);
        if(dp < 0)
            return vec3{ 0 };

        float gamma = dp * light->mLuminosity / dist;
        gamma = std::min(gamma, 1.0f);

        return intrInfo.object->mMaterial.mColor * gamma;
    }


    return { 1, 0, 0 };
}

Pixel Scene::renderPixel(uint32_t x, uint32_t y) {
    if(mConfig.debugPixel && y == mConfig.debugY && x == mConfig.debugX) {
        std::cout << "Debug pixel\n";
    }

    // FIXME: non optimal
    Ray ray = mCamera.getRay(mConfig, 2.0f * x / mConfig.renderWidth - 1.0f, 2.0f * y / mConfig.renderHeight - 1.0f);
    IntersectInfo finalIntr = { false };
    for(auto& obj : mObjects) {
        IntersectInfo intersectInfo;
        bool intr = obj->intersect(ray, intersectInfo);
        if(!intr) {
            continue;
        }
        bool replaceFinalIntrWithCurrent = intersectInfo.intersect && (finalIntr.intersect == false || finalIntr.distance > intersectInfo.distance);
        if(replaceFinalIntrWithCurrent) {
            finalIntr = intersectInfo;
        }
    }

    if(!finalIntr.intersect) {
        return mConfig.renderBGColor;
    }

    return colorVec3ToPix(illuminatePoint(finalIntr));
    /*depthe*/ return colorVec3ToPix(vec3{1,1,1} * finalIntr.distance / 5.0f);
    /*uv*/ return colorVec3ToPix(vec3{finalIntr.uv.x, finalIntr.uv.y, 0});
    /*photon partition*/ return colorVec3ToPix(vec3{
            std::floor(finalIntr.uv.x * finalIntr.object->mUSize) / finalIntr.object->mUSize,
            std::floor(finalIntr.uv.y * finalIntr.object->mVSize) / finalIntr.object->mVSize,0 });
}
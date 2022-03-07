#include "Scene.h"


Scene::Scene(Config& config) :
        mConfig(config),
        mCamera(config) {
    mObjects.emplace_back(new Sphere(vec3{-0.5, 1, -5}, 1));
    mObjects.emplace_back(new Sphere(vec3{5, 2, -8}, 1));
}
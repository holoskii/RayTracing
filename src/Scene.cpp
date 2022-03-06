#include "Scene.h"


Scene::Scene(Config& config) :
        mConfig(config),
        mCamera(config) {
    mObjects.emplace_back(new Sphere({0, 0, -20}, 1));
    mObjects.emplace_back(new Sphere({5, 3, -4}, 1));
}
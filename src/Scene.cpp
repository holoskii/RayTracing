#include "Scene.h"


Scene::Scene(Config& config) :
        mConfig(config),
        mCamera(config) {
    mObjects.emplace_back(new Sphere(vec3{0, 0, -20}, 1));
    mObjects.emplace_back(new Sphere(vec3{5, 3, -4}, 1));
}
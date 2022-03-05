#pragma once

#include <iostream>

#include "Common.h"
#include "Config.h"


/// Get math
/// GLM


/// Workflow
/// Direct renderer: Camera, object, rays, intersections
/// Photon maps: a lot of photons divided in blocks
/// Photon mapping: light source will emit photons to photon maps
/// Upon intersection camera rays will gather information
///     from photon maps they hit

/// TODO
/// Environmental map
/// Check previous version for obvious improvement points
/// Browse another implementations, see what mine is lacking
/// Compare low precision to high precision
/// Add option to lock camera on center
///


/// Starting point, direction







/// Material will store color, type, texture, normal maps, coefficient of refraction, etc...
/// It shall define how object will interact with light ray intersection
/// If may need to launch additional rays (reflections)
/// Params like metallic, specular, etc... see Materials at UE and Unity
class Material {
public:

};

/// Defiles position, orientation, type (sphere, mesh, cylinder, etc)
/// have to check if the object intersects with the ray
class Object {
public:
    vec3 mPos;

    Object(const vec3& pos) :
        mPos(pos) {}

    virtual bool intersect(const Ray& ray) = 0;
};

class Sphere : protected Object{
public:
    float mRad;

    Sphere(const vec3& pos, float rad) :
        Object(pos),
        mRad(rad) {}

    bool intersect(const Ray& ray) override {
        float t;
        float t0, t1; // solutions for t if the ray intersects
#if 1
        // geometric solution
        vec3 L = mPos - ray.mPos;
        float tca = glm::dot(L, ray.mDir);
        // if (tca < 0) return false;
        float d2 = glm::dot(L, L) - tca * tca;
        if (d2 > mRad * mRad) return false;
        float thc = sqrt(mRad * mRad - d2);
        t0 = tca - thc;
        t1 = tca + thc;
#else
        // analytic solution
        vec3 L = ray.mPos - pos;
        float a = dir.dotProduct(dir);
        float b = 2 * dir.dotProduct(L);
        float c = L.dotProduct(L) - radius2;
        if (!solveQuadratic(a, b, c, t0, t1)) return false;
#endif
        if (t0 > t1) std::swap(t0, t1);

        if (t0 < 0) {
            t0 = t1; // if t0 is negative, let's use t1 instead
            if (t0 < 0) return false; // both t0 and t1 are negative
        }

        t = t0;

        return true;
    }
};

/// Like the object + intensity and color
/// Will have to emit photons in photon mapping stage
class LightSource : public Object {

};

/// Like LightSource, but never intersects with the ray
class PointLightSource : public Object {

};


void objectTest();
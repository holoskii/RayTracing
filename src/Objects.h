#pragma once

#include <iostream>

#include "Common.h"
#include "Config.h"

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

class Sphere : public Object{
public:
    Sphere(vec3 pos, float rad) :
            Object(pos),
            mRad(rad),
            mRadSqr(rad * rad) {}

private:
    float mRad;
    float mRadSqr;

    bool intersect(const Ray& ray) override {
        // Solve equation: origin + direction * distance = point of intersection
        // We need to solve the triangle that formed  with radius perpendicular to ray direction,
        // vector from ray origin to center and ray direction vector

        vec3 hypotenuse = mPos - ray.mPos;
        float adjSide = glm::dot(hypotenuse, ray.mDir);
        // Pointing another direction
        if (adjSide < 0) {
            return false;
        }
        // Pythagorean  theorem
        float oppositeSide = glm::dot(hypotenuse, hypotenuse) - adjSide * adjSide;
        // Out of sphere radius
        if (oppositeSide > mRadSqr) {
            return false;
        }
        // Distance from intersection point and opposite side
        float dist = sqrtf(mRadSqr - oppositeSide);

        // Distance from ray origin to intersection point
        float result = adjSide - dist;
        if(result < 0) {
            result = adjSide + dist;
        }
        if(result < 0) {
            return false;
        }

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
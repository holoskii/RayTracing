#pragma once

#include <iostream>

#include "Common.h"
#include "Config.h"

class Object;

class IntersectInfo {
public:
    bool        intersect;
    float       distance;
    Object*     object;
    vec3        normal;
};

/// Material will store color, type, texture, normal maps, coefficient of refraction, etc...
/// It shall define how object will interact with light ray intersection
/// If may need to launch additional rays (reflections)
/// Params like metallic, specular, etc... see Materials at UE and Unity
class Material {
public:
    Material() {
        mColor = { 255, 0, 0, 255 };
    }


    Material(vec4& color) :
            mColor(color) {}
private:
    vec4 mColor;
};

/// Defiles position, orientation, type (sphere, mesh, cylinder, etc)
/// have to check if the object intersects with the ray
class Object {
public:
    vec3        mPos;
    Material    mMaterial;

    Object(const vec3& pos) :
            mPos(pos) {}

    virtual IntersectInfo intersect(const Ray& ray) = 0;
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

    IntersectInfo intersect(const Ray& ray) override {
        // Solve equation: origin + direction * distance = point of intersection
        // We need to solve the triangle that formed  with radius perpendicular to ray direction,
        // vector from ray origin to center and ray direction vector

        vec3 hypotenuse = mPos - ray.mPos;
        float adjSide = glm::dot(hypotenuse, ray.mDir);
        // Pointing another direction
        if (adjSide < 0) {
            return { false };
        }
        // Pythagorean  theorem
        float oppositeSide = glm::dot(hypotenuse, hypotenuse) - adjSide * adjSide;
        // Out of sphere radius
        if (oppositeSide > mRadSqr) {
            return { false };
        }
        // Distance from intersection point and opposite side
        float dist = sqrtf(mRadSqr - oppositeSide);

        // Distance from ray origin to intersection point
        float resultDistance = adjSide - dist;
        if(resultDistance < 0) {
            resultDistance = adjSide + dist;
        }
        if(resultDistance < 0) {
            return {false};
        }

        return {
            true,
            resultDistance,
            this,
            glm::normalize(ray.mDir * resultDistance - mPos)
        };
    }
};

/// Like the object + intensity and color
/// Will have to emit photons in photon mapping stage
class LightSource {
public:
    vec3 mPos;
};

/// Like LightSource, but never intersects with the ray
class PointLightSource : public Object {
public:
    vec3 color;
    float luminosity;
};


void objectTest();
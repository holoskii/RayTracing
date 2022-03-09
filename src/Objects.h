#pragma once

#include <iostream>

#include "Common.h"
#include "Config.h"

class Object;

static float offsetScalar = 1e-4;

class IntersectInfo {
public:
    IntersectInfo(bool _intersect)
        : intersect(_intersect) {}

    IntersectInfo(bool _intersect, float _distance, Object* _object, vec3 _normal, vec3 _point)
        : intersect(_intersect)
        , distance(_distance)
        , object(_object)
        , normal(_normal)
        , point(_point) {}

    bool        intersect;
    float       distance;
    Object*     object;
    vec3        normal;
    vec3        point;
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
    std::string mName;

    Object(const std::string&& name, const vec3& pos) :
            mName(std::move(name)), mPos(pos) {}

    Object(const vec3& pos) :
            Object("None", pos) {}



    virtual IntersectInfo intersect(const Ray& ray) = 0;
};

class Sphere : public Object{
public:
    Sphere(std::string&& name, vec3 pos, float rad) :
            Object(std::move(name), pos),
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

        vec3 point = ray.mPos + ray.mDir * resultDistance;
        vec3 normal = glm::normalize(point - mPos);

        return {
            true,
            resultDistance,
            this,
            normal,
            point + normal * offsetScalar
        };
    }
};

class Rectangle : public Object {
public:
    Rectangle(std::string&& name, vec3 pos, vec3 side1, vec3 side2) :
            Object(std::move(name), pos),
            mSide1Len(glm::length(side1)),
            mSide1(glm::normalize(side1)),
            mSide2Len(glm::length(side2)),
            mSide2(side2) {
        mNormal = glm::cross(side1, side2);
    }

    vec3 mSide1;
    float mSide1Len;
    vec3 mSide2;
    float mSide2Len;
    vec3 mNormal;

    IntersectInfo intersect(const Ray& ray) override {
        if(glm::dot(ray.mDir, mNormal) > 0) {
            return { false };
        }

        // resultDistance = ((mPos - ray.mPos).N) / (ray.dir.N)
        float resultDistance = glm::dot(mPos - ray.mPos, mNormal) / glm::dot(ray.mDir, mNormal);
        vec3 point = ray.mPos + resultDistance * ray.mDir;
        vec3 p = (point) - mPos;

        auto inRange = [](float v, float max){ return v >= 0 && v <= max; };

        if(inRange(glm::dot(p, mSide1), mSide1Len) && inRange(glm::dot(p, mSide2), mSide2Len)) {
            return {
                    true,
                    resultDistance,
                    this,
                    mNormal,
                    point + offsetScalar * mNormal
            };
        }

        return { false };
    }
};

/// Like the object + intensity and color
/// Will have to emit photons in photon mapping stage
class LightSource {
public:
    vec3 mPos;
    vec3 mColor;
    float mLuminosity;


    LightSource(vec3 pos, vec3 color, float luminosity)
        :mPos(pos), mColor(color), mLuminosity(luminosity) {

    }
};

/// Like LightSource, but never intersects with the ray
class PointLightSource : public LightSource {
public:

    PointLightSource(vec3 pos, vec3 color, float luminosity)
        : LightSource(pos, color, luminosity) {
    }
};


void objectTest();
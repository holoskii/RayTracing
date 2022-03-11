#pragma once

#include "Common.h"
#include "Config.h"
#include <iostream>

class Object;

constexpr float offsetScalar = 1e-6f;

class IntersectInfo {
public:
    IntersectInfo() = default;

    IntersectInfo(bool _intersect)
        : intersect(_intersect) {}

    IntersectInfo(bool _intersect, float _distance, Object* _object, vec3 _normal, vec3 _point, vec2 _uv)
        : intersect(_intersect)
        , distance(_distance)
        , object(_object)
        , normal(_normal)
        , point(_point)
        , uv(_uv) {}

    bool        intersect;
    float       distance;
    Object*     object;
    vec3        normal;
    vec3        point;
    vec2        uv;
};

/// Material will store color, type, texture, normal maps, coefficient of refraction, etc...
/// It shall define how object will interact with light ray intersection
/// If may need to launch additional rays (reflections)
/// Params like metallic, specular, etc... see Materials at UE and Unity
class Material {
public:
    Material(vec3&& color)
        : mColor(color) {}

    Material()
        : Material(vec3{ 255, 255, 255 }) {}

    vec3 mColor;
};

/// Defiles position, orientation, type (sphere, mesh, cylinder, etc)
/// have to check if the object intersects with the ray
class Object {
public:
    vec3        mPos;
    Material    mMaterial;
    std::string mName;

    Object(const std::string&& name, const vec3& pos, const Material& material)
        : mName(std::move(name))
        , mPos(pos)
        , mMaterial(material) {}

    virtual bool intersect(const Ray& ray, IntersectInfo& finalIntersect) = 0;
};

class Sphere : public Object{
public:
    Sphere(std::string&& name, vec3 pos, const Material& material, float rad)
        : Object(std::move(name), pos, material)
        , mRad(rad)
        , mRadSqr(rad * rad) {}

private:
    float mRad;
    float mRadSqr;

    bool intersect(const Ray& ray, IntersectInfo& finalIntersect) override;
};

class Rectangle : public Object {
public:
    Rectangle(std::string&& name, vec3 pos, const Material& material, vec3 side1, vec3 side2)
        : Object(std::move(name), pos, material)
        , mSide1Len(glm::length(side1))
        , mSide1(glm::normalize(side1))
        , mSide2Len(glm::length(side2))
        , mSide2(glm::normalize(side2)) {
        mNormal = glm::cross(side1, side2);
    }

    vec3 mNormal;
    vec3 mSide1;
    vec3 mSide2;
    float mSide1Len;
    float mSide2Len;

    bool intersect(const Ray& ray, IntersectInfo& finalIntersect) override;
};

/// Position, Color, Brightness
class LightSource {
public:
    vec3 mPos;
    vec3 mColor;
    float mLuminosity;

    LightSource(vec3 pos, vec3 color, float luminosity)
        : mPos(pos)
        , mColor(color)
        , mLuminosity(luminosity) {}
};

/// "Invisible", can't be intersected with the ray
class PointLightSource : public LightSource {
public:
    PointLightSource(vec3 pos, vec3 color, float luminosity)
        : LightSource(pos, color, luminosity) {}
};



void objectTest();
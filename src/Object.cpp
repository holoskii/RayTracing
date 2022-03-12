#include "Objects.h"

bool Sphere::intersect(const Ray& ray, IntersectInfo& finalIntersect) {
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
    float resultDistance = adjSide - dist;
    if(resultDistance < 0) {
        resultDistance = adjSide + dist;
    }
    if(resultDistance < 0) {
        return false;
    }

    vec3 point = ray.mPos + ray.mDir * resultDistance;
    vec3 normal = glm::normalize(point - mPos);

    float side = normal.z;
    float u = std::atan2(normal.x, normal.z) / (2 * pi) + 0.5;
    float v = std::asin(normal.y) / pi + 0.5;

    finalIntersect = {
            true,
            resultDistance,
            this,
            normal,
            point + normal * offsetScalar,
            vec2{u, v}
    };

    return true;
}

bool Rectangle::intersect(const Ray& ray, IntersectInfo& finalIntersect) {
    if(glm::dot(ray.mDir, mNormal) > 0) {
        return false;
    }

    float resultDistance = glm::dot(mPos - ray.mPos, mNormal) / glm::dot(ray.mDir, mNormal);
    vec3 point = ray.mPos + resultDistance * ray.mDir;
    vec3 p = (point) - mPos;

    auto inRange = [](float v, float max){ return v >= 0 && v <= max; };

    float u = glm::dot(p, mSide1) / mSide1Len;
    float v = glm::dot(p, mSide2) / mSide2Len;

    if(!inRange(u, 1.0f) || !inRange(v, 1.0f)) {
        return false;
    }

    // u or v can't be exactly 1
    u = std::min(u, 0.999f);
    v = std::min(v, 0.999f);

    finalIntersect = {
        true,
        resultDistance,
        this,
        mNormal,
        point + offsetScalar * mNormal,
        vec2{ u, v }
    };

    return true;
}

void objectTest() {}

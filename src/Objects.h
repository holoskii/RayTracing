#pragma once

#include <iostream>

#include <glm/common.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Common.h"


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
class Ray {
public:
    vec4 mPos;
    vec4 mDir;
    /*Ray(vec4 & pos, vec4 & dir) :
        mPos(pos),
        mDir(dir)
    {

    }*/
};
/*
class Config {

};
*/
/// Direction, FOV (from config)
/// Used to get camera rays
/// Move and turn calculated here
class Camera {
public:
    float fov = pi / 4; // 90 deg FOV
    vec4 rot; // rotation, only y and x is used
    vec4 pos;

    /*Ray getRay(Config & config, uint32_t x, uint32_t y) {
        return {};
    }*/

};





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

};


/// Like the object + intensity and color
/// Will have to emit photons in photon mapping stage
class LightSource : public Object {

};

/// Like LightSource, but never intersects with the ray
class PointLightSource : public Object {

};


void objectTest();
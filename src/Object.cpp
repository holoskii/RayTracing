#include "Objects.h"

#include "Camera.h"

#include <glm/gtx/string_cast.hpp>

void objectTest() {
    Camera cam;
//    std::cout << glm::to_string(cam.lookDir) << '\n';
//    std::cout << glm::to_string(cam.rightVec) << '\n';
//    std::cout << glm::to_string(cam.upVec) << '\n';

    /*Config config;


    std::cout << glm::to_string(cam.getRay(config, 1.0f, 1.0f).mDir) << '\n';
    std::cout << glm::to_string(cam.getRay(config, 1.0f, -1.0f).mDir) << '\n';
    std::cout << glm::to_string(cam.getRay(config, 0.0f, 1.0f).mDir) << '\n';
    std::cout << glm::to_string(cam.getRay(config, 0.0f, -1.0f).mDir) << '\n';
    */


    /*https://learnopengl.com/Getting-started/Camera
    float xRot = pi / 4;
    float yRot = pi / 2;

    mat4 xRotMat = {
            {1, 0, 0, 0},
            {0, cosf(xRot), -sinf(xRot), 0},
            {0, sinf(xRot), cosf(xRot), 0},
            {0, 0, 0, 1}
    };
    mat4 yRotMat = {
            {cosf(yRot), 0, -sinf(yRot), 0},
            {0, 1, 0, 0},
            {sinf(yRot), 0, cosf(yRot), 0},
            {0, 0, 0, 1}
    };

    mat4 resultMat = yRotMat * xRotMat;

    vec4 dir = {0, 0, -1, 1};
    std::cout << glm::to_string(resultMat * dir) << '\n';

    */
}



/*

 y - counerzclockwize, right hand
z .
y ^
x ->












*/
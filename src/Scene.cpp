#include "Scene.h"


Scene::Scene(Config& config) :
        mConfig(config),
        mCamera(config) {
    mObjects.emplace_back(new Sphere(vec3{0.3, 0.2, -0.5}, 0.2));
    mObjects.emplace_back(new Sphere(vec3{0.7, 0.3, -0.3}, 0.3));

    // floor
    mObjects.emplace_back(
        new Rectangle(
            vec3{0,0,0},
            vec3{1,0,0},
            vec3{0,0,-1}
        )
    );

    // ceiling
    mObjects.emplace_back(
        new Rectangle(
            vec3{0,1,0},
            vec3{0,0,-1},
            vec3{1,0,0}
        )
    );

    // back
    mObjects.emplace_back(
        new Rectangle(
            vec3{0,0,-1},
            vec3{1,0,0},
            vec3{0,1,0}
        )
    );

    // left
    mObjects.emplace_back(
            new Rectangle(
                    vec3{0,0,0},
                    vec3{0,0,-1},
                    vec3{0,1,0}
            )
    );

    // right
    mObjects.emplace_back(
            new Rectangle(
                    vec3{1,0,0},
                    vec3{0,1,0},
                    vec3{0,0,-1}
            )
    );
}
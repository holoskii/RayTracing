#pragma once

#include "RenderBasics.h"
#include "Scene.h"

class RenderCore {
public:
    RenderCore(Config & config, Scene& scene);
    void renderTile(Tile & tile, bool & running);
    void setBuffer(pixel * imageBuffer);

    Config& mConfig;
    Scene& mScene;
    pixel * mImageBuffer = nullptr;
};


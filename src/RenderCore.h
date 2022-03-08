#pragma once

#include "RenderBasics.h"
#include "Scene.h"

class RenderCore {
public:
    RenderCore(Config & config, Scene& scene);
    void renderTile(Tile & tile, bool & running);
    void setBuffer(Pixel * imageBuffer);

    Config& mConfig;
    Scene& mScene;
    Pixel * mImageBuffer = nullptr;
};


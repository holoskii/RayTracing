#pragma once

#include "RenderBasics.h"

class RenderCore {
public:
    explicit RenderCore(Config & config);
    void renderTile(Tile & tile, bool & running);
    void setBuffer(pixel * imageBuffer);

    Config & mConfig;
    pixel * mImageBuffer = nullptr;
};


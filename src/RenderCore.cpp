#include "RenderCore.h"

#include <thread>

RenderCore::RenderCore(Config & config) :
        mConfig(config)
{
}

void RenderCore::renderTile(Tile & tile, bool & running) {
    assert(running);
    assert(mImageBuffer);
    uint32_t & width = mConfig.renderWidth;
    uint32_t & height = mConfig.renderHeight;
    if(mConfig.renderMode == RenderMode::Fast) {
        for(int i = tile.yStart; i < tile.yEnd && running; i++) {
            for(int j = tile.xStart; j < tile.xEnd && running; j++) {
                mImageBuffer[(i * width + j)] = {
                        (uint8_t) (255 * i / height),
                        (uint8_t) (255 * j / width),
                        (uint8_t) 0,
                        (uint8_t) 255
                };
            }
        }
    }
    else if(mConfig.renderMode == RenderMode::Slow) {
        for(int i = tile.yStart; i < tile.yEnd && running; i++) {
            if(rand() % 10 == 0)
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            for(int j = tile.xStart; j < tile.xEnd && running; j++) {
                long sum = 0;
                mImageBuffer[(i * width + j)] = {
                        (uint8_t)  (rand() % 255),
                        (uint8_t) (rand() % 255),
                        (uint8_t) (rand() % 255),
                        (uint8_t) 255
                };
            }
        }
    }
    else {
        for(int i = tile.yStart; i < tile.yEnd && running; i++) {
            for(int j = tile.xStart; j < tile.xEnd && running; j++) {
                mImageBuffer[(i * width + j)] = { 0, 0, 0, 0};
            }
        }
    }
}


void RenderCore::setBuffer(pixel * imageBuffer) {
    mImageBuffer = imageBuffer;
}



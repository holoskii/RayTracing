#include "RenderCore.h"

#include "Camera.h"
#include "Objects.h"

#include <thread>

RenderCore::RenderCore(Config& config, Scene& scene) :
        mConfig(config),
        mScene(scene) {}

void RenderCore::renderTile(Tile & tile, bool & running) {
    assert(running);
    assert(mImageBuffer);
    if(mConfig.renderMode == RenderMode::Fast) {
        for(int i = tile.yStart; i < tile.yEnd && running; i++) {
            for(int j = tile.xStart; j < tile.xEnd && running; j++) {
                mImageBuffer[(i * mConfig.renderWidth + j)] = {
                        (uint8_t) (255 * i / mConfig.renderHeight),
                        (uint8_t) (255 * j / mConfig.renderWidth),
                        (uint8_t) 0
                };
            }
        }
    }
    else if(mConfig.renderMode == RenderMode::Slow) {
        for(int i = tile.yStart; i < tile.yEnd && running; i++) {
            if(rand() % 17 == 0)
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            for(int j = tile.xStart; j < tile.xEnd && running; j++) {
                long sum = 0;
                mImageBuffer[(i * mConfig.renderWidth + j)] = {
                        (uint8_t)  (rand() % 255),
                        (uint8_t) (rand() % 255),
                        (uint8_t) (rand() % 255)
                };
            }
        }
    }
    else {
        for(uint32_t i = tile.yStart; i < tile.yEnd && running; i++) {
            for(uint32_t j = tile.xStart; j < tile.xEnd && running; j++) {
                auto res = mScene.renderPixel(j, i);
                mImageBuffer[(i * mConfig.renderWidth + j)] = res;
            }
        }
    }
}


void RenderCore::setBuffer(Pixel * imageBuffer) {
    mImageBuffer = imageBuffer;
}



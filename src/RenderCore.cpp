#include "RenderCore.h"

#include "Camera.h"
#include "Objects.h"

#include <thread>

RenderCore::RenderCore(Config & config) :
        mConfig(config) {}

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
        Camera cam;
        Sphere s1({0, 0, -20}, 1);
        Sphere s2({5, 5, -4}, 1);

        pixel intr{255, 0, 0, 255};
        pixel nintr{0, 0, 0, 255};

        for(int i = tile.yStart; i < tile.yEnd && running; i++) {
            for(int j = tile.xStart; j < tile.xEnd && running; j++) {
                Ray ray = cam.getRay(mConfig, 2.0f * j / mConfig.renderWidth - 1.0f, 2.0f * i / mConfig.renderHeight - 1.0f);
                uint8_t val = (uint8_t)((ray.mDir.y + 1) / 2 * 255);
                //mImageBuffer[(i * width + j)] = {val, 0, 0, 255};

                mImageBuffer[(i * width + j)] = s1.intersect(ray) || s2.intersect(ray) ? intr : nintr;
            }
        }
    }
}


void RenderCore::setBuffer(pixel * imageBuffer) {
    mImageBuffer = imageBuffer;
}



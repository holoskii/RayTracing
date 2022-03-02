#pragma once

#include "Config.h"

#include <cstdint>

struct pixel {
    uint8_t r, g, b, a;
};

class Tile {
public:
    int32_t xStart, xEnd;
    int32_t yStart, yEnd;

    static std::vector<Tile> getSpiralTiles(Config& config) {
        auto getTile = [&](int32_t x, int32_t y){
            Tile tile{};
            tile.xStart = x * config.tileSize;
            tile.xEnd   = std::min((x + 1) * config.tileSize, config.renderWidth);
            tile.yStart = y * config.tileSize;
            tile.yEnd   = std::min((y + 1) * config.tileSize, config.renderHeight);
            return tile;
        };

        int32_t tilesWidth = config.renderWidth / config.tileSize + (config.renderWidth % config.tileSize != 0);
        int32_t tilesHeight = config.renderHeight / config.tileSize + (config.renderHeight % config.tileSize != 0);
        std::vector<Tile> result(tilesWidth * tilesHeight);

        int height = 0, width = 0;
        uint64_t vecIndex = tilesWidth * tilesHeight - 1;
        while (height < tilesHeight && width < tilesWidth) {
            for (int i = width; i < tilesWidth; ++i)
                result[vecIndex--] = getTile(i, height);
            height++;

            for (int i = height; i < tilesHeight; ++i)
                result[vecIndex--] = getTile(tilesWidth - 1, i);
            tilesWidth--;

            if (height < tilesHeight) {
                for (int i = tilesWidth - 1; i >= width; --i)
                    result[vecIndex--] = getTile(i, tilesHeight - 1);
                tilesHeight--;
            }

            if (width < tilesWidth) {
                for (int i = tilesHeight - 1; i >= height; --i)
                    result[vecIndex--] = getTile(width, i);
                width++;
            }
        }
        return result;
    }
};

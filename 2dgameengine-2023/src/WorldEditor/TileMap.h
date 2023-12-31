#ifndef TILEMAP_H
#define TILEMAP_H
#include <string>

struct TileMap { std::string mapFilePath; int numRows; int numCols; int tileSize; double scale; };

#endif
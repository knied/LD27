//
//  Level.cpp
//  LD27
//
//  Created by Kristof Niederholtmeyer on 24.08.13.
//  Copyright (c) 2013 Kristof Niederholtmeyer. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "Level.h"
#include <QImage>

////////////////////////////////////////////////////////////////////////////////

Level::Level(const std::string& name) : _default_tile(LevelWall) {
    QImage img(name.c_str());
    _width = img.width();
    _height = img.height();
    
    _data.resize(_width * _height);
    
    for (unsigned int y = 0; y < _height; ++y) {
        for (unsigned int x = 0; x < _width; ++x) {
            QRgb rgb = img.pixel(x, y);
            LevelTile tile = tile_for_rgb(qRed(rgb),
                                          qGreen(rgb),
                                          qBlue(rgb));
            if (tile == LevelSpawn) {
                _spawn = Position(x,_height - y - 1);
                _data[(_height - y - 1) * _width + x].tile = LevelFloor;
            } else if (tile == LevelMonsterSpawn) {
                _monster_spawns.push_back(Position(x,_height - y - 1));
                _data[(_height - y - 1) * _width + x].tile = LevelFloor;
            } else {
                _data[(_height - y - 1) * _width + x].tile = tile;
            }
            _data[(_height - y - 1) * _width + x].known = false;
        }
    }
}

const LevelTile& Level::at(int x, int y) const {
    if (x < 0 || x > _width || y < 0 || y > _height) {
        return _default_tile;
    }
    return _data[y * _width + x].tile;
}

LevelTile Level::tile_for_rgb(unsigned char r,
                              unsigned char g,
                              unsigned char b) const {
    if (r == 0 && g == 0 && b == 0) {
        return LevelWall;
    }
    
    if (r == 255 && g == 255 && b == 255) {
        return LevelFloor;
    }
    
    if (r == 255 && g == 0 && b == 0) {
        return LevelSpawn;
    }
    
    if (r == 0 && g == 255 && b == 0) {
        return LevelMonsterSpawn;
    }
    
    return LevelWall;
}

bool Level::known_at(int x, int y) const {
    if (x < 0 || x > _width || y < 0 || y > _height) {
        return false;
    }
    return _data[y * _width + x].known;
}

void Level::set_known(int x, int y) {
    if (x < 0 || x > _width || y < 0 || y > _height) {
        return;
    }
    _data[y * _width + x].known = true;
}

const Position& Level::spawn() const {
    return _spawn;
}

const std::vector<Position>& Level::monster_spawns() const {
    return _monster_spawns;
}

////////////////////////////////////////////////////////////////////////////////

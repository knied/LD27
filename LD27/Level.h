//
//  Level.h
//  LD27
//
//  Created by Kristof Niederholtmeyer on 24.08.13.
//  Copyright (c) 2013 Kristof Niederholtmeyer. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __LD27__Level__
#define __LD27__Level__

////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>

////////////////////////////////////////////////////////////////////////////////

typedef enum {
    LevelFloor,
    LevelWall,
    LevelSpawn
} LevelTile;

class Level {
    struct Data {
        LevelTile tile;
        bool known;
    };
    
    std::vector<Data> _data;
    
    unsigned int _width;
    unsigned int _height;
    
    int _spawn_x;
    int _spawn_y;
    
    LevelTile _default_tile;
    
    LevelTile tile_for_rgb(unsigned char r,
                           unsigned char g,
                           unsigned char b) const;
    
public:
    Level(const std::string& name);
    
    const LevelTile& at(int x, int y) const;
    bool known_at(int x, int y) const;
    void set_known(int x, int y);
    
    int spawn_x() const;
    int spawn_y() const;
};

////////////////////////////////////////////////////////////////////////////////

#endif /* defined(__LD27__Level__) */

////////////////////////////////////////////////////////////////////////////////

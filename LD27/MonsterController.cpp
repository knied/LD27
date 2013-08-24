//
//  MonsterController.cpp
//  LD27
//
//  Created by Kristof Niederholtmeyer on 24.08.13.
//  Copyright (c) 2013 Kristof Niederholtmeyer. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "MonsterController.h"

////////////////////////////////////////////////////////////////////////////////

MonsterController::MonsterController(const EntityComponentHandle<Orientation>& orientation,
                                     const EntityComponentHandle<Tile>& tile)
: _orientation(orientation), _tile(tile), _action_timer(0.0f) {
    
}

void MonsterController::set_handles(const EntityComponentHandle<Orientation>& orientation,
                                    const EntityComponentHandle<Tile>& tile) {
    _orientation = orientation;
    _tile = tile;
}

void MonsterController::set_position(const Position& position) {
    _orientation->position = position;
}

enum MonsterMode {
    Idle,
    Follow,
    Flee
};

void MonsterController::update(float dt, const Level& level, PlayerController& player) {
    //if (_action_timer >= 0.2f) {
    //    _action_timer = 0.0f;
        
        MonsterMode mode = Follow;
        
        Line line_of_sight(_orientation->position, player.position());
    
    if (line_of_sight.size() <= 2) {
        player.hurt();
    }
        bool visible = player.point_visible(_orientation->position.x, _orientation->position.y, level);
        bool in_sight = true;
        for (unsigned int i = 0; i < line_of_sight.size(); ++i) {
            if (level.at(line_of_sight[i].x,line_of_sight[i].y) == LevelWall) {
                in_sight = false;
                break;
            }
        }
        
        if (player.torch_active() && visible) {
            mode = Flee;
        } else {
            if (!in_sight) {
                mode = Idle;
            }
        }
        
        int dir = 0;
        Direction direction = NORTH;
        Position dest = _orientation->position;
        
        if (mode == Idle) {
            dir = rand() % 4;
            switch (dir) {
                case 0: direction = NORTH; break;
                case 1: direction = SOUTH; break;
                case 2: direction = EAST; break;
                case 3: direction = WEST; break;
                default: direction = NORTH; break;
            }
            
            if (rand() % 8 == 0) {
                switch (direction) {
                    case NORTH: dest.y += 1; break;
                    case SOUTH: dest.y -= 1; break;
                    case EAST: dest.x += 1; break;
                    case WEST: dest.x -= 1; break;
                }
            }
        } else if (mode == Follow) {
            if (line_of_sight.size() > 1) {
                if (rand() % 8 == 0) {
                    dest = line_of_sight[1];
                }
                direction = line_of_sight.direction();
            }
        } else {
            direction = rand_flee(line_of_sight.direction());
            switch (direction) {
                case NORTH: dest.y += 1; break;
                case SOUTH: dest.y -= 1; break;
                case EAST: dest.x += 1; break;
                case WEST: dest.x -= 1; break;
            }
        }
        
        if (level.at(dest.x, dest.y) == LevelFloor) {
            _orientation->position = dest;
        }
        
        _orientation->direction = direction;
        
        bool eyes_visible = !visible && in_sight;
        
        // update the tile
        if (eyes_visible) {
            _tile->symbol_color = Color(100, 83, 31, 255);
        } else if (!visible) {
            _tile->symbol_color = Color(0, 0, 0, 0);
        } else {
            _tile->symbol_color = Color(0, 0, 0, 255);
        }
        _tile->background_color = Color(0, 0, 0, 0);
        
        if (eyes_visible) {
            _tile->symbol = 35; // eyes in the dark
        } else {
            switch (_orientation->direction) {
                case NORTH:
                    _tile->symbol = 33; // monster from behind
                    _tile->flip_x = false;
                    _tile->flip_y = false;
                    break;
                case SOUTH:
                    _tile->symbol = 32; // monster from behind
                    _tile->flip_x = false;
                    _tile->flip_y = false;
                    break;
                case EAST:
                    _tile->symbol = 34; // monster from behind
                    _tile->flip_x = false;
                    _tile->flip_y = false;
                    break;
                case WEST:
                    _tile->symbol = 34; // monster from behind
                    _tile->flip_x = true;
                    _tile->flip_y = false;
                    break;
            }
        }
    //}
    //_action_timer += dt;
}

Direction MonsterController::rand_flee(Direction from) const {
    int r = rand() % 5;
    if (from == NORTH) {
        switch (r) {
            case 0: return WEST; break;
            case 1: return EAST; break;
            default: return SOUTH; break;
        }
    } else if (from == SOUTH) {
        switch (r) {
            case 0: return WEST; break;
            case 1: return EAST; break;
            default: return NORTH; break;
        }
    } else if (from == EAST) {
        switch (r) {
            case 0: return NORTH; break;
            case 1: return SOUTH; break;
            default: return WEST; break;
        }
    } else {
        switch (r) {
            case 0: return NORTH; break;
            case 1: return SOUTH; break;
            default: return EAST; break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

//
//  PlayerController.cpp
//  LD27
//
//  Created by Kristof Niederholtmeyer on 24.08.13.
//  Copyright (c) 2013 Kristof Niederholtmeyer. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "PlayerController.h"
#include <math.h>

////////////////////////////////////////////////////////////////////////////////

PlayerController::PlayerController(const EntityComponentHandle<Orientation>& orientation,
                                   const EntityComponentHandle<Tile>& tile)
: _orientation(orientation), _tile(tile),
_north_control(KEY_ARROW_UP), _south_control(KEY_ARROW_DOWN),
_east_control(KEY_ARROW_RIGHT), _west_control(KEY_ARROW_LEFT),
_torch_control(KEY_T),
_fov(PI*0.4),
_view(3), _torch_timer(0.0f), _health_timer(0.0f),
_health(10) {
    
}

void PlayerController::set_handles(const EntityComponentHandle<Orientation>& orientation,
                                   const EntityComponentHandle<Tile>& tile) {
    _orientation = orientation;
    _tile = tile;
}

void PlayerController::set_position(const Position& position) {
    _orientation->position = position;
}

void PlayerController::handle_keyboard_event(const KeyEvent& event) {
    _north_control.handle_event(event);
    _south_control.handle_event(event);
    _east_control.handle_event(event);
    _west_control.handle_event(event);
    
    _torch_control.handle_event(event);
}

void PlayerController::update(float dt, const Level& level) {
    _health_timer += dt;
    if (_health_timer > 1.0f) {
        _health_timer = 0.0f;
        if (_health < 10) {
            _health++;
        }
    }
    
    
    Position dest = _orientation->position;
    
    if (_north_control.pressed()
        || _north_control.down()) {
        dest.y += 1;
        _orientation->direction = NORTH;
    }
    if (_south_control.pressed()
        || _south_control.down()) {
        dest.y -= 1;
        _orientation->direction = SOUTH;
    }
    if (_east_control.pressed()
        || _east_control.down()) {
        dest.x += 1;
        _orientation->direction = EAST;
    }
    if (_west_control.pressed()
        || _west_control.down()) {
        dest.x -= 1;
        _orientation->direction = WEST;
    }
    
    if (_torch_control.pressed()) {
        //_health--;
        _view = 12;
    }
    
    _north_control.clear();
    _south_control.clear();
    _east_control.clear();
    _west_control.clear();
    
    _torch_control.clear();
    
    if (level.at(dest.x, dest.y) == LevelFloor
        && point_visible(dest.x, dest.y, level)) {
        _orientation->position = dest;
    }
    
    if (_torch_timer > 10.0f) {
        _torch_timer = 0.0f;
        _view = 3;
    }
    
    if (_view == 12) {
        _torch_timer += dt;
    }
    
    // update the tile
    _tile->symbol_color = Color(63, 127, 63, 255);
    _tile->background_color = Color(0, 0, 0, 0);
    
    switch (_orientation->direction) {
        case NORTH:
            _tile->symbol = 1; // guy from behind
            _tile->flip_x = false;
            _tile->flip_y = false;
            break;
        case SOUTH:
            _tile->symbol = 0; // guy from behind
            _tile->flip_x = false;
            _tile->flip_y = false;
            break;
        case EAST:
            _tile->symbol = 2; // guy from behind
            _tile->flip_x = false;
            _tile->flip_y = false;
            break;
        case WEST:
            _tile->symbol = 2; // guy from behind
            _tile->flip_x = true;
            _tile->flip_y = false;
            break;
    }
}

int PlayerController::point_visible(int x, int y, const Level& level) const {
    int offset_x = 0;
    int offset_y = 0;
    
    switch (_orientation->direction) {
        case NORTH: offset_y = -2; break;
        case SOUTH: offset_y = 2; break;
        case EAST: offset_x = -2; break;
        case WEST: offset_x = 2; break;
    }
    
    if (_orientation->position.x + offset_x == x && _orientation->position.y + offset_y == y) {
        return 0;
    }
    
    int dx = x - _orientation->position.x;
    int dy = y - _orientation->position.y;
    
    /*if (dx * dx + dy * dy < 2*2) {
        return true;
    }*/
    if (dx * dx + dy * dy > _view*_view) {
        return 0;
    }
    
    /*if (x == _orientation->position.x && y == _orientation->position.y) {
        return true;
    }*/
    
    float phi = atan2(dy - offset_y,dx - offset_x);
    
    if (_orientation->direction == NORTH) {
        phi -= PI * 0.5f;
    } else if (_orientation->direction == SOUTH) {
        phi += PI * 0.5f;
    } else if (_orientation->direction == WEST) {
        if (phi > 0.0f) {
            phi -= PI;
        } else {
            phi += PI;
        }
    }
    
    if (phi >= -_fov && phi <= _fov) {
        Line view_ray(Position(_orientation->position.x,
                               _orientation->position.y),
                      Position(x, y));
        for (unsigned int i = 0; i < view_ray.size(); ++i) {
            Position current_position = view_ray[i];
            if (level.at(current_position.x,
                         current_position.y) == LevelWall) {
                if (current_position.x != x || current_position.y != y) {
                    return 0;
                }
            }
        }
        return view_ray.size();
    }
    return 0;
}

bool PlayerController::torch_active() const {
    return _view == 12;
}

const Position& PlayerController::position() const {
    return _orientation->position;
}

void PlayerController::hurt() {
    if (_health > 0) {
        _health--;
    }
}

float PlayerController::blood_factor() const {
    return 1.0f - (float)_health / 10.0f;
}

////////////////////////////////////////////////////////////////////////////////

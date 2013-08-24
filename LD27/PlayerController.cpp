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
_fov(PI*0.4) {
    
}

void PlayerController::update_tile() {
    _tile->symbol_color = Color(127, 255, 127, 255);
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

void PlayerController::update_direction() {
    if (_north_control.pressed()) {
        std::cout << "north" << std::endl;
        _orientation->direction = NORTH;
    }
    if (_south_control.pressed()) {
        std::cout << "south" << std::endl;
        _orientation->direction = SOUTH;
    }
    if (_east_control.pressed()) {
        std::cout << "east" << std::endl;
        _orientation->direction = EAST;
    }
    if (_west_control.pressed()) {
        std::cout << "west" << std::endl;
        _orientation->direction = WEST;
    }
}

void PlayerController::set_handles(const EntityComponentHandle<Orientation>& orientation,
                                   const EntityComponentHandle<Tile>& tile) {
    _orientation = orientation;
    _tile = tile;
    update_tile();
}

void PlayerController::handle_keyboard_event(const KeyEvent& event) {
    _north_control.handle_event(event);
    _south_control.handle_event(event);
    _east_control.handle_event(event);
    _west_control.handle_event(event);
    update_direction();
    update_tile();
    _north_control.clear();
    _south_control.clear();
    _east_control.clear();
    _west_control.clear();
}

bool PlayerController::point_visible(int x, int y) const {
    if (x == _orientation->position.x && y == _orientation->position.y) {
        return true;
    }
    
    float _lower_bound = -_fov;
    float _higher_bound = _fov;
    
    float phi = atan2(y-_orientation->position.y,x-_orientation->position.x);
    
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
    
    if (phi > _lower_bound && phi < _higher_bound) {
        return true;
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////

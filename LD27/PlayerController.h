//
//  PlayerController.h
//  LD27
//
//  Created by Kristof Niederholtmeyer on 24.08.13.
//  Copyright (c) 2013 Kristof Niederholtmeyer. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __LD27__PlayerController__
#define __LD27__PlayerController__

////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "Types.h"
#include "EntityComponent.h"
#include "Graphics.h"
#include "Input.h"
#include "Level.h"

////////////////////////////////////////////////////////////////////////////////

class PlayerController {
    EntityComponentHandle<Orientation> _orientation;
    EntityComponentHandle<Tile> _tile;
    
    Control _north_control;
    Control _south_control;
    Control _east_control;
    Control _west_control;
    
    Control _torch_control;
    
    float _fov;
    int _view;
    float _torch_timer;
    float _health_timer;
    
    int _health;
    
public:
    PlayerController(const EntityComponentHandle<Orientation>& orientation = EntityComponentHandle<Orientation>(),
                     const EntityComponentHandle<Tile>& tile = EntityComponentHandle<Tile>());
    
    void set_handles(const EntityComponentHandle<Orientation>& orientation,
                     const EntityComponentHandle<Tile>& tile);
    
    void set_position(const Position& position);
    
    void handle_keyboard_event(const KeyEvent& event);
    void update(float dt, const Level& level);
    
    int point_visible(int x, int y, const Level& level) const;
    bool torch_active() const;
    
    const Position& position() const;
    
    void hurt();
    
    float blood_factor() const;
};

////////////////////////////////////////////////////////////////////////////////

#endif /* defined(__LD27__PlayerController__) */

////////////////////////////////////////////////////////////////////////////////


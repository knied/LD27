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

////////////////////////////////////////////////////////////////////////////////

class PlayerController {
    EntityComponentHandle<Orientation> _orientation;
    EntityComponentHandle<Tile> _tile;
    
    Control _north_control;
    Control _south_control;
    Control _east_control;
    Control _west_control;
    
    void update_tile();
    void update_direction();
    
    float _fov;
    
public:
    PlayerController(const EntityComponentHandle<Orientation>& orientation = EntityComponentHandle<Orientation>(),
                     const EntityComponentHandle<Tile>& tile = EntityComponentHandle<Tile>());
    
    void set_handles(const EntityComponentHandle<Orientation>& orientation,
                     const EntityComponentHandle<Tile>& tile);
    
    void handle_keyboard_event(const KeyEvent& event);
    
    bool point_visible(int x, int y) const;
};

////////////////////////////////////////////////////////////////////////////////

#endif /* defined(__LD27__PlayerController__) */

////////////////////////////////////////////////////////////////////////////////


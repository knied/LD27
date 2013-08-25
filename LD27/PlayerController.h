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
#include "Sound.h"

////////////////////////////////////////////////////////////////////////////////

class PlayerController {
    EntityComponentHandle<Orientation> _orientation;
    EntityComponentHandle<Tile> _tile;
    
    Control _north_control;
    Control _south_control;
    Control _east_control;
    Control _west_control;
    
    Control _torch_control;
    
    Sound _step_sound;
    Sound _torch_sound;
    Sound _key_sound;
    Sound _hit0_sound;
    Sound _hit1_sound;
    Sound _door_sound;
    
    float _fov;
    int _view;
    float _torch_timer;
    float _health_timer;
    
    int _health;
    
    float _show_locked;
    float _show_move_instructions;
    bool _fade_out_move_instructions;
    float _show_torch_instructions;
    bool _fade_out_torch_instructions;
    
    float _show_inventory;
    
    unsigned int _keys;
    unsigned int _torches;
    
public:
    PlayerController(const EntityComponentHandle<Orientation>& orientation = EntityComponentHandle<Orientation>(),
                     const EntityComponentHandle<Tile>& tile = EntityComponentHandle<Tile>());
    
    void set_handles(const EntityComponentHandle<Orientation>& orientation,
                     const EntityComponentHandle<Tile>& tile);
    
    void set_position(const Position& position);
    
    void handle_keyboard_event(const KeyEvent& event);
    void update(float dt, Level& level);
    
    int point_visible(int x, int y, const Level& level) const;
    bool torch_active() const;
    
    const Position& position() const;
    
    void hurt();
    
    float blood_factor() const;
    float show_locked() const;
    float show_move_instructions() const;
    float show_torch_instructions() const;
    float show_inventory() const;
    
    void add_torch();
    void add_key();
    unsigned int keys() const {return _keys;}
    unsigned int torches() const {return _torches;}
    
    void restart();
};

////////////////////////////////////////////////////////////////////////////////

#endif /* defined(__LD27__PlayerController__) */

////////////////////////////////////////////////////////////////////////////////


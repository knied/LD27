//
//  Game.h
//  LD27
//
//  Created by Kristof Niederholtmeyer on 17.08.13.
//  Copyright (c) 2013 Kristof Niederholtmeyer. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __LD27__Game__
#define __LD27__Game__

////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>

#include "Graphics.h"
#include "Input.h"
#include "Sound.h"
#include "EntityComponent.h"
#include "Types.h"
#include "PlayerController.h"
#include "Level.h"

////////////////////////////////////////////////////////////////////////////////

class Game {
public:
    enum { GRID_WIDTH = 25 };
    enum { GRID_HEIGHT = 25 };
    
private:
    GridView<GRID_WIDTH, GRID_HEIGHT> _grid_view;
    
    EntityComponent<Orientation> _orientation_component;
    EntityComponent<Tile> _tile_component;
    
    std::vector<Entity> _entities;
    std::vector<EntityComponentHandle<Orientation> > _orientations;
    std::vector<EntityComponentHandle<Tile> > _tiles;
    
    PlayerController _player_controller;
    
    EntityComponentHandle<Orientation> _camera_orientation;
    
    Level* _level;
    
    /*TextControl _test_text_control;
    float _cursor_timer;
    //Sound _flush_sound;
    
    EntityComponent<Tile> _tile_component;
    EntityComponent<Position> _position_component;
    
    std::vector<Entity> _entities;
    std::vector<EntityComponentHandle<Tile> > _entity_tiles;
    std::vector<EntityComponentHandle<Position> > _entity_positions;
    
    Control _player_left_control;
    Control _player_right_control;
    
    Entity _player;
    EntityComponentHandle<Tile> _player_tile;
    EntityComponentHandle<Position> _player_position;
    
    void spawn_entity(const Tile& tile, const Position& position);*/
    
public:
    Game();
    ~Game();
    
    void handle_text_event(const std::string& text);
    void handle_keyboard_event(const KeyEvent& event);
    void update(float dt);
    const GridView<GRID_WIDTH, GRID_HEIGHT>& grid_view() const;
};

////////////////////////////////////////////////////////////////////////////////

#endif /* defined(__LD27__Game__) */

////////////////////////////////////////////////////////////////////////////////

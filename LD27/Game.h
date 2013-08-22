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

////////////////////////////////////////////////////////////////////////////////

struct Position {
    int x, y;
    Position() : x(0), y(0) {}
    Position(int x, int y) : x(x), y(y) {}
};

////////////////////////////////////////////////////////////////////////////////

class Game {
public:
    enum { GRID_WIDTH = 24 };
    enum { GRID_HEIGHT = 24 };
    
private:
    GridView<GRID_WIDTH, GRID_HEIGHT> _grid_view;
    
    TextControl _test_text_control;
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
    
    void spawn_entity(const Tile& tile, const Position& position);
    
public:
    Game();
    
    void handle_text_event(const std::string& text);
    void handle_keyboard_event(const KeyEvent& event);
    void update(float dt);
    const GridView<GRID_WIDTH, GRID_HEIGHT>& grid_view() const;
};

////////////////////////////////////////////////////////////////////////////////

#endif /* defined(__LD27__Game__) */

////////////////////////////////////////////////////////////////////////////////

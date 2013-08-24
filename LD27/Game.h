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
#include "MonsterController.h"
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
    std::vector<MonsterController> _monster_controllers;
    
    EntityComponentHandle<Orientation> _camera_orientation;
    
    Level* _level;
    unsigned char wall_symbol(int x, int y, bool& flip_x, bool& flip_y) const;
    
    bool _game_over;
    
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

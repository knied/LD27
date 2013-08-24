//
//  Game.cpp
//  LD27
//
//  Created by Kristof Niederholtmeyer on 17.08.13.
//  Copyright (c) 2013 Kristof Niederholtmeyer. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "Game.h"

#include <sstream>

////////////////////////////////////////////////////////////////////////////////

Game::Game() : _level(0) {
    // spawn player
    _entities.push_back(create_entity());
    _orientations.push_back(EntityComponentHandle<Orientation>(&_orientation_component, _entities.back()));
    _tiles.push_back(EntityComponentHandle<Tile>(&_tile_component, _entities.back()));
    _player_controller.set_handles(_orientations.back(), _tiles.back());
    _camera_orientation = _orientations.back();
    
    _level = new Level(":/Level.png");
    _player_controller.set_position(Position(_level->spawn_x(),
                                             _level->spawn_y()));
}

Game::~Game() {
    if (_level) {
        delete _level;
    }
}

void Game::handle_text_event(const std::string& text) {
    
}

void Game::handle_keyboard_event(const KeyEvent& event) {
    _player_controller.handle_keyboard_event(event);
}

void Game::update(float dt) {
    _player_controller.update(dt, *_level);
    
    // clear screen
    
    
    unsigned char d = 0;
    int visability_factor = 8;
    unsigned char fire_off = 0;
    if (_player_controller.torch_active()) {
        d = rand() % 15;
        visability_factor = 2;
        fire_off = 10;
    }
    
    
    for (unsigned int y = 0; y < GRID_HEIGHT; ++y) {
        for (unsigned int x = 0; x < GRID_WIDTH; ++x) {
            int world_x = x - (GRID_WIDTH - 1) / 2 + _camera_orientation->position.x;
            int world_y = y - (GRID_HEIGHT - 1) / 2 + _camera_orientation->position.y;
            
            unsigned char symbol = 136;
            
            unsigned char off = 0;
            unsigned char boff = 0;
            if (_level->known_at(world_x, world_y)) {
                boff += 10;
                if (_level->at(world_x, world_y) == LevelWall) {
                    off += 20;
                    boff += 10;
                }
            }
            
            int visability = _player_controller.point_visible(world_x,
                                                              world_y,
                                                              *_level);
            if (visability > 0) {
                boff -= visability*visability_factor;
                _grid_view.set_tile(x, y, symbol,
                                    Color(40+off+boff+fire_off,
                                          40+off+boff+fire_off,
                                          40+off+boff-d),
                                    Color(40+boff+fire_off,
                                          40+boff+fire_off,
                                          40+boff-d));
                _level->set_known(world_x, world_y);
            } else {
                if (off) {
                    off -= 15;
                    boff -= 10;
                }
                _grid_view.set_tile(x, y, symbol,
                                    Color(0+off+boff,0+off+boff,0+off+boff),
                                    Color(0+boff,0+boff,0+boff));
            }
        }
    }
    
    /*if (_player_controller.draw_move_select()) {
        for (unsigned int i = _player_controller.move_select_start(); i < _player_controller.move_select().size(); ++i) {
            int view_x = _player_controller.move_select()[i].x - _camera_orientation->position.x;
            int view_y = _player_controller.move_select()[i].y - _camera_orientation->position.y;
            
            int grid_x = view_x + (GRID_WIDTH - 1) / 2;
            int grid_y = view_y + (GRID_HEIGHT - 1) / 2;
            
            _grid_view.set_tile(grid_x, grid_y, 0, Color(40,80,40), Color(30,60,30));
        }
    }*/
    
    // draw entities
    for (unsigned int i = 0; i < _entities.size(); ++i) {
        int view_x = _orientations[i]->position.x - _camera_orientation->position.x;
        int view_y = _orientations[i]->position.y - _camera_orientation->position.y;
        
        int grid_x = view_x + (GRID_WIDTH - 1) / 2;
        int grid_y = view_y + (GRID_HEIGHT - 1) / 2;
        
        if (grid_x >= 0 && grid_x < GRID_WIDTH && grid_y >= 0 && grid_y < GRID_HEIGHT) {
            Color background_color = _grid_view.background_color(grid_x, grid_y);
            
            // draw this entity
            _grid_view.set_tile(grid_x, grid_y, *_tiles[i]);
            
            _grid_view.set_background_color(grid_x, grid_y, alpha_blend(background_color, _grid_view.background_color(grid_x, grid_y)));
        }
    }
}

const GridView<Game::GRID_WIDTH, Game::GRID_HEIGHT>& Game::grid_view() const {
    return _grid_view;
}

////////////////////////////////////////////////////////////////////////////////

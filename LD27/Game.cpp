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

Game::Game() {
    // spawn player
    _entities.push_back(create_entity());
    _orientations.push_back(EntityComponentHandle<Orientation>(&_orientation_component, _entities.back()));
    _tiles.push_back(EntityComponentHandle<Tile>(&_tile_component, _entities.back()));
    _player_controller.set_handles(_orientations.back(), _tiles.back());
    _camera_orientation = _orientations.back();
}

void Game::handle_text_event(const std::string& text) {
    
}

void Game::handle_keyboard_event(const KeyEvent& event) {
    _player_controller.handle_keyboard_event(event);
}

void Game::update(float dt) {
    // clear screen
    for (unsigned int y = 0; y < GRID_HEIGHT; ++y) {
        for (unsigned int x = 0; x < GRID_WIDTH; ++x) {
            int world_x = x - (GRID_WIDTH - 1) / 2 + _camera_orientation->position.x;
            int world_y = y - (GRID_HEIGHT - 1) / 2 + _camera_orientation->position.y;
            if (_player_controller.point_visible(world_x, world_y)) {
                _grid_view.set_tile(x, y, 0, Color(40,40,40), Color(40,40,40));
            } else {
                _grid_view.set_tile(x, y, 0, Color(0,0,0), Color(0,0,0));
            }
        }
    }
    
    //std::cout << "line:" << std::endl;
    Line test_line(Position(-10, -3), Position(5, 2));
    for (unsigned int i = 0; i < test_line.size(); ++i) {
        //std::cout << i << " " << test_line[i].x << " " << test_line[i].y << std::endl;
        
        int view_x = test_line[i].x - _camera_orientation->position.x;
        int view_y = test_line[i].y - _camera_orientation->position.y;
        
        int grid_x = view_x + (GRID_WIDTH - 1) / 2;
        int grid_y = view_y + (GRID_HEIGHT - 1) / 2;
        
        _grid_view.set_tile(grid_x, grid_y, 0, Color(40,20,20), Color(40,20,20));
    }
    
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

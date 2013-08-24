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

Game::Game() : _level(0), _game_over(false) {
    // spawn player
    _entities.push_back(create_entity());
    _orientations.push_back(EntityComponentHandle<Orientation>(&_orientation_component, _entities.back()));
    _tiles.push_back(EntityComponentHandle<Tile>(&_tile_component, _entities.back()));
    _player_controller.set_handles(_orientations.back(), _tiles.back());
    _camera_orientation = _orientations.back();
    
    _level = new Level(":/Level.png");
    _player_controller.set_position(_level->spawn());
    
    for (unsigned int i = 0; i < _level->monster_spawns().size(); ++i) {
        _entities.push_back(create_entity());
        _orientations.push_back(EntityComponentHandle<Orientation>(&_orientation_component, _entities.back()));
        _tiles.push_back(EntityComponentHandle<Tile>(&_tile_component, _entities.back()));
        _monster_controllers.push_back(MonsterController(_orientations.back(), _tiles.back()));
        _monster_controllers.back().set_position(_level->monster_spawns()[i]);
    }
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

unsigned char Game::wall_symbol(int x, int y, bool& flip_x, bool& flip_y) const {
    flip_x = false;
    flip_y = false;
    unsigned char base = 144;
    unsigned char symbol = base + 11;
    
    if (_level->at(x, y) == LevelWall) {
        bool left = _level->at(x-1, y) == LevelWall;
        bool right = _level->at(x+1, y) == LevelWall;
        bool top = _level->at(x, y+1) == LevelWall;
        bool bottom = _level->at(x, y-1) == LevelWall;
        
        if (left && right && top && bottom) symbol = base + 2;
        else if (left && right && bottom) symbol = base + 3;
        else if (left && right && top) { symbol = base + 3; flip_y = true; }
        else if (top && bottom && left) symbol = base + 4;
        else if (top && bottom && left) { symbol = base + 4; flip_x = true; }
        else if (left && right) symbol = base + 0;
        else if (top && bottom) symbol = base + 1;
        else if (right && bottom) symbol = base + 5;
        else if (left && bottom) { symbol = base + 5; flip_x = true; }
        else if (right && top) { symbol = base + 5; flip_y = true; }
        else if (left && top) { symbol = base + 5; flip_x = true; flip_y = true; }
        else if (left) symbol = base + 6;
        else if (right) { symbol = base + 6; flip_x = true; }
        else if (top) symbol = base + 7;
        else if (bottom) { symbol = base + 7; flip_y = true; }
        else symbol = base + 8;
    }
    
    return symbol;
}

void Game::update(float dt) {
    if (!_game_over) {
        _player_controller.update(dt, *_level);
        for (unsigned int i = 0; i < _monster_controllers.size(); ++i) {
            _monster_controllers[i].update(dt, *_level, _player_controller);
        }
    }
    
    unsigned char d = 0;
    int visibility_factor = 8;
    unsigned char fire_off = 0;
    if (_player_controller.torch_active()) {
        d = rand() % 15;
        visibility_factor = 2;
        fire_off = 10;
    }
    
    for (unsigned int y = 0; y < GRID_HEIGHT; ++y) {
        for (unsigned int x = 0; x < GRID_WIDTH; ++x) {
            int world_x = x - (GRID_WIDTH - 1) / 2 + _camera_orientation->position.x;
            int world_y = y - (GRID_HEIGHT - 1) / 2 + _camera_orientation->position.y;
            
            bool flip_x;
            bool flip_y;
            unsigned char symbol = wall_symbol(world_x, world_y, flip_x, flip_y);
            
            unsigned char off = 0;
            unsigned char boff = 0;
            if (_level->known_at(world_x, world_y)) {
                boff += 15;
                if (_level->at(world_x, world_y) == LevelWall) {
                    off += 22;
                    boff += 20;
                }
            }
            
            int visible = _player_controller.point_visible(world_x,
                                                              world_y,
                                                              *_level);
            if (visible > 0) {
                boff -= visible*visibility_factor;
                _grid_view.set_tile(x, y, symbol,
                                    Color(40+off+boff+fire_off+d,
                                          40+off+boff+fire_off+d,
                                          40+off+boff),
                                    Color(40+boff+fire_off+d,
                                          40+boff+fire_off+d,
                                          40+boff),
                                    flip_x, flip_y);
                _level->set_known(world_x, world_y);
            } else {
                if (off) {
                    off -= 15;
                    boff -= 15;
                }
                _grid_view.set_tile(x, y, symbol,
                                    Color(0+off+boff,0+off+boff,0+off+boff),
                                    Color(0+boff,0+boff,0+boff),
                                    flip_x, flip_y);
            }
        }
    }
    
    // draw entities
    for (unsigned int i = 0; i < _entities.size(); ++i) {
        for (unsigned int j = 0; j < _entities.size(); ++j) {
            if (i == j) {
                continue;
            }
            if (_orientations[j]->position.x == _orientations[i]->position.x &&
                _orientations[j]->position.y == _orientations[i]->position.y) {
                _orientations[i]->position.x = _orientations[i]->position.old_x;
                _orientations[i]->position.y = _orientations[i]->position.old_y;
            }
        }
        
        int view_x = _orientations[i]->position.x - _camera_orientation->position.x;
        int view_y = _orientations[i]->position.y - _camera_orientation->position.y;
        
        int grid_x = view_x + (GRID_WIDTH - 1) / 2;
        int grid_y = view_y + (GRID_HEIGHT - 1) / 2;
        
        if (grid_x >= 0 && grid_x < GRID_WIDTH && grid_y >= 0 && grid_y < GRID_HEIGHT) {
            Color background_color = _grid_view.background_color(grid_x, grid_y);
            Color symbol_color = _grid_view.symbol_color(grid_x, grid_y);
            
            // draw this entity
            _grid_view.set_tile(grid_x, grid_y, *_tiles[i]);
            
            _grid_view.set_background_color(grid_x, grid_y, alpha_blend(background_color, _grid_view.background_color(grid_x, grid_y)));
            _grid_view.set_symbol_color(grid_x, grid_y, alpha_blend(symbol_color, _grid_view.symbol_color(grid_x, grid_y)));
        }
    }
    
    Color blood_color(200, 0, 0);
    for (unsigned int y = 0; y < GRID_HEIGHT; ++y) {
        for (unsigned int x = 0; x < GRID_WIDTH; ++x) {
            _grid_view.set_background_color(x, y, blend(_grid_view.background_color(x, y), blood_color, _player_controller.blood_factor()));
            _grid_view.set_symbol_color(x, y, blend(_grid_view.symbol_color(x, y), blood_color, _player_controller.blood_factor()));
        }
    }
    
    if (_player_controller.blood_factor() == 1.0f) {
        _game_over = true;
    }
    
    if (_game_over) {
        _grid_view.draw_text(8, 12, "GAME OVER", Color(255,255,255));
    }
}

const GridView<Game::GRID_WIDTH, Game::GRID_HEIGHT>& Game::grid_view() const {
    return _grid_view;
}

////////////////////////////////////////////////////////////////////////////////

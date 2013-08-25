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

Game::Game() : _next_level_sound("next_level"), _level(0), _game_over(false), _game_finished(false), _current_level(0), _level_fade_out(1.0f), _level_finished(false), _game_finish_fade_out(0.0f) {
    // spawn player
    /*_entities.push_back(create_entity());
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
    
    _keys = _level->key_spawns();
    _torches = _level->torch_spawns();*/
    _level_names.push_back(":/Level0.png");
    _level_names.push_back(":/Level1.png");
    _level_names.push_back(":/Level2.png");
    load_level(_current_level);
}

Game::~Game() {
    if (_level) {
        delete _level;
    }
}

void Game::load_level(unsigned int level) {
    if (level < _level_names.size()) {
        _entities.clear();
        _orientations.clear();
        _tiles.clear();
        _monster_controllers.clear();
        
        _entities.push_back(create_entity());
        _orientations.push_back(EntityComponentHandle<Orientation>(&_orientation_component, _entities.back()));
        _tiles.push_back(EntityComponentHandle<Tile>(&_tile_component, _entities.back()));
        _player_controller.set_handles(_orientations.back(), _tiles.back());
        _camera_orientation = _orientations.back();
        
        _level = new Level(_level_names[level].c_str());
        _player_controller.set_position(_level->spawn());
        
        for (unsigned int i = 0; i < _level->monster_spawns().size(); ++i) {
            _entities.push_back(create_entity());
            _orientations.push_back(EntityComponentHandle<Orientation>(&_orientation_component, _entities.back()));
            _tiles.push_back(EntityComponentHandle<Tile>(&_tile_component, _entities.back()));
            _monster_controllers.push_back(MonsterController(_orientations.back(), _tiles.back()));
            _monster_controllers.back().set_position(_level->monster_spawns()[i]);
        }
        
        _keys = _level->key_spawns();
        _torches = _level->torch_spawns();
    } else {
        _game_finished = true;
    }
}

void Game::handle_text_event(const std::string& text) {
    
}

void Game::handle_keyboard_event(const KeyEvent& event) {
    _player_controller.handle_keyboard_event(event);
}

unsigned char Game::symbol(int x, int y, bool& flip_x, bool& flip_y) const {
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
    } else if (_level->at(x, y) == LevelDoor) {
        symbol = 94;
    } else if (_level->at(x, y) == LevelExit) {
        symbol = 31;
    }
    
    return symbol;
}

void Game::colors(int x, int y,
                  Color& symbol_color,
                  Color& background_color) const {
    Color floor_symbol_color(80,80,80);
    Color floor_background_color(80,80,80);
    
    Color wall_symbol_color(255,255,255);
    Color wall_background_color(127,127,127);
    
    Color door_symbol_color(143,130,117);
    Color door_background_color(99,62,26);
    
    Color exit_symbol_color(255,255,255);
    
    if (_level->known_at(x, y)) {
        if (_level->at(x, y) == LevelWall) {
            symbol_color = symbol_color + wall_symbol_color;
            background_color = background_color + wall_background_color;
        } else if (_level->at(x, y) == LevelFloor) {
            symbol_color = symbol_color + floor_symbol_color;
            background_color = background_color + floor_background_color;
        } else if (_level->at(x, y) == LevelDoor) {
            symbol_color = symbol_color + door_symbol_color;
            background_color = background_color + door_background_color;
        } else if (_level->at(x, y) == LevelExit) {
            symbol_color = symbol_color + exit_symbol_color;
            background_color = background_color + floor_background_color;
        }
    }
}

void Game::update(float dt) {
    if (!_game_over && !_game_finished && !_level_finished) {
        _player_controller.update(dt, *_level);
        for (unsigned int i = 0; i < _monster_controllers.size(); ++i) {
            _monster_controllers[i].update(dt, *_level, _player_controller);
        }
    }
    
    if (_level->at(_player_controller.position().x,
                   _player_controller.position().y) == LevelExit) {
        _level_finished = true;
    }
    
    if (_level_finished) {
        if (_level_fade_out < 1.0f) {
            _level_fade_out += dt;
            if (_level_fade_out > 1.0f) {
                _level_finished = false;
                _next_level_sound.setGain(0.5f);
                _next_level_sound.play(1);
                load_level(++_current_level);
                _level_fade_out = 1.0f;
            }
        }
    } else {
        if (_level_fade_out > 0.0f) {
            _level_fade_out -= dt;
            if (_level_fade_out < 0.0f) {
                _level_fade_out = 0.0f;
            }
        }
    }
    
    unsigned int i = 0;
    for (; i < _keys.size(); ++i) {
        if (_player_controller.position().x == _keys[i].x
            && _player_controller.position().y == _keys[i].y) {
            break;
        }
    }
    if (i < _keys.size()) {
        _player_controller.add_key();
        _keys.erase(_keys.begin()+i);
    }
    
    i = 0;
    for (; i < _torches.size(); ++i) {
        if (_player_controller.position().x == _torches[i].x
            && _player_controller.position().y == _torches[i].y) {
            break;
        }
    }
    if (i < _torches.size()) {
        _player_controller.add_torch();
        _torches.erase(_torches.begin()+i);
    }
    
    unsigned char d = 0;
    //int visibility_factor = 8;
    //unsigned char fire_off = 0;
    if (_player_controller.torch_active()) {
        d = rand() % 40;
        //visibility_factor = 2;
        //fire_off = 10;
    }
    
    for (unsigned int y = 0; y < GRID_HEIGHT; ++y) {
        for (unsigned int x = 0; x < GRID_WIDTH; ++x) {
            int world_x = x - (GRID_WIDTH - 1) / 2 + _camera_orientation->position.x;
            int world_y = y - (GRID_HEIGHT - 1) / 2 + _camera_orientation->position.y;
            
            int visible = _player_controller.point_visible(world_x,
                                                           world_y,
                                                           *_level);
            
            Color light(31,31,31);
            if (visible) {
                unsigned char visibility_factor = 20;
                
                light = light + Color(100,100,100);
                if (_player_controller.torch_active()) {
                    light = light + Color(80-d,80-d,40-d);
                    visibility_factor = 10;
                }
                
                Color visible_offset(visible*visibility_factor,
                                     visible*visibility_factor,
                                     visible*visibility_factor);
                
                light = light - visible_offset;
            }
            
            
            if (visible > 0) {
                _level->set_known(world_x, world_y);
            }
            
            bool flip_x;
            bool flip_y;
            Color symbol_color;
            Color background_color;
            unsigned char symbol = this->symbol(world_x, world_y,
                                                flip_x, flip_y);
            colors(world_x, world_y,
                   symbol_color,
                   background_color);
            
            if (visible) {
                for (unsigned int i = 0; i < _keys.size(); ++i) {
                    if (_keys[i].x == world_x && _keys[i].y == world_y) {
                        symbol_color = Color(196,175,0);
                        symbol = 79;
                    }
                }
                
                for (unsigned int i = 0; i < _torches.size(); ++i) {
                    if (_torches[i].x == world_x && _torches[i].y == world_y) {
                        symbol_color = Color(215,167,22);
                        symbol = 110;
                    }
                }
            }
            
            _grid_view.set_tile(x, y, symbol,
                                symbol_color * light,
                                background_color * light,
                                flip_x, flip_y);
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
    
    if (_game_finished) {
        _game_finish_fade_out += dt;
        if (_game_finish_fade_out > 1.0f) {
            _game_finish_fade_out = 1.0f;
        }
    }
    
    Color blood_color(200, 0, 0);
    
    float fade_factor = 0.0f;
    Color fade_color;
    
    if (!_game_finished) {
        if (_player_controller.blood_factor() > 0.0f) {
            fade_factor = _player_controller.blood_factor();
            fade_color = blood_color;
        }
        if (_level_fade_out > 0.0f) {
            fade_factor = _level_fade_out;
            fade_color = Color(0,0,0);
        }
    } else {
        fade_factor = _game_finish_fade_out;
        fade_color = Color(255,255,255);
    }
    
    if (_player_controller.show_inventory() > 0.0f) {
        Color background_color = blend(_grid_view.background_color(i, 24), Color(0,0,0), _player_controller.show_inventory());
        Color symbol_color = blend(_grid_view.symbol_color(i, 24), Color(0,0,0), _player_controller.show_inventory());
        for (unsigned int i = 0; i < 25; ++i) {
            _grid_view.set_tile(i, 24, Tile(4, symbol_color, background_color));
        }
        
        for (unsigned int i = 0; i < _player_controller.torches(); ++i) {
            Color torch_symbol_color = blend(_grid_view.symbol_color(i, 24), Color(215,167,22), _player_controller.show_inventory());
            _grid_view.set_tile(i, 24, Tile(110, torch_symbol_color, background_color));
        }
        
        for (unsigned int i = 0; i < _player_controller.keys(); ++i) {
            Color key_symbol_color = blend(_grid_view.symbol_color(24-i, 24), Color(196,175,0), _player_controller.show_inventory());
            _grid_view.set_tile(24-i, 24, Tile(79, key_symbol_color, background_color));
        }
    }
    
    for (unsigned int y = 0; y < GRID_HEIGHT; ++y) {
        for (unsigned int x = 0; x < GRID_WIDTH; ++x) {
            if (_game_finished) {
                _grid_view.set_background_color(x, y, blend(Color(0,0,0), fade_color, fade_factor));
                _grid_view.set_symbol_color(x, y, blend(Color(0,0,0), fade_color, fade_factor));
            } else {
                _grid_view.set_background_color(x, y, blend(_grid_view.background_color(x, y), fade_color, fade_factor));
                _grid_view.set_symbol_color(x, y, blend(_grid_view.symbol_color(x, y), fade_color, fade_factor));
            }
        }
    }
    
    if (_game_over) {
        _grid_view.draw_text(8, 12, "GAME OVER", Color(255,255,255));
    } else if (_game_finished) {
        _grid_view.draw_text(5, 14, "CONGRATULATIONS.", Color(0,0,0));
        _grid_view.draw_text(2, 12, "YOU MADE IT OUT OF THE", Color(0,0,0));
        _grid_view.draw_text(9, 10, "DUNGEON.", Color(0,0,0));
    } else {
        if (_player_controller.blood_factor() == 1.0f) {
            _game_over = true;
        }
        
        if (_player_controller.show_locked() > 0.0f) {
            unsigned char locked_color = 255.0f * _player_controller.show_locked();
            _grid_view.draw_text(1, 1, "LOCKED", Color(locked_color,
                                                       locked_color,
                                                       locked_color,
                                                       locked_color));
        }
        
        if (_player_controller.show_move_instructions() > 0.0f) {
            unsigned char locked_color = 255.0f * _player_controller.show_move_instructions();
            _grid_view.draw_text(1, 1, "ARROW KEYS TO MOVE", Color(locked_color,
                                                                   locked_color,
                                                                   locked_color,
                                                                   locked_color));
        }
        
        if (_player_controller.show_torch_instructions() > 0.0f) {
            unsigned char locked_color = 255.0f * _player_controller.show_torch_instructions();
            _grid_view.draw_text(1, 1, "SPACE TO LIGHT A TORCH", Color(locked_color,
                                                                       locked_color,
                                                                       locked_color,
                                                                       locked_color));
        }
    }
}

const GridView<Game::GRID_WIDTH, Game::GRID_HEIGHT>& Game::grid_view() const {
    return _grid_view;
}

////////////////////////////////////////////////////////////////////////////////

//
//  Game.cpp
//  LD27
//
//  Created by Kristof Niederholtmeyer on 17.08.13.
//  Copyright (c) 2013 Kristof Niederholtmeyer. All rights reserved.
//

#include "Game.h"

#include <sstream>

Game::Game() : _test_text_control(""), _cursor_timer(0.0f), _flush_sound("flush") {
    
}

void Game::handle_text_event(const std::string& text) {
    _test_text_control.handle_text_event(text);
}

void Game::handle_keyboard_event(const KeyEvent& event) {
    _cursor_timer = 0.0f;
    _test_text_control.handle_event(event);
}

void Game::update(float dt) {
    if (!_flush_sound.is_playing()) {
        _flush_sound.play(1);
    }
    
    for (unsigned int y = 0; y < GRID_HEIGHT; ++y) {
        for (unsigned int x = 0; x < GRID_WIDTH; ++x) {
            _grid_view.set_tile(x, y, rand() % 256, Color(rand() % 256, rand() % 256, rand() % 256, 255), Color(rand() % 256, rand() % 256, rand() % 256, 255), rand() % 2, rand() % 2);
        }
    }
    _grid_view.draw_text(0, 0, _test_text_control.text(), Color(255,255,255), Color(0,0,0));
    
    Color symbol_color(255,255,255);
    Color background_color(0,0,0);
    _grid_view.set_symbol_color(_test_text_control.cursor(), 0, symbol_color);
    _grid_view.set_background_color(_test_text_control.cursor(), 0, background_color);
    if (_cursor_timer < 0.25f) {
        _grid_view.set_flip(_test_text_control.cursor(), 0, false, false);
        _grid_view.set_symbol(_test_text_control.cursor(), 0, 219);
    }
    if (_cursor_timer > 0.5f) {
        _cursor_timer = 0.0f;
    }
    //_grid_view.set_tile(_test_text_control.cursor(), 0, 219, symbol_color, background_color);
    _cursor_timer += dt;
    
    // Show FPS in the top left corner
    std::stringstream fps_ss;
    unsigned int fps = 1.0f / dt;
    fps_ss << fps;
    _grid_view.draw_text(0, GRID_HEIGHT-1, fps_ss.str(), Color(255,255,255));
}

const GridView<Game::GRID_WIDTH, Game::GRID_HEIGHT>& Game::grid_view() const {
    return _grid_view;
}
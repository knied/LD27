//
//  Game.cpp
//  LD27
//
//  Created by Kristof Niederholtmeyer on 17.08.13.
//  Copyright (c) 2013 Kristof Niederholtmeyer. All rights reserved.
//

#include "Game.h"

#include <sstream>

Game::Game() : _test_text_control("HELLO") {
    
}

void Game::handle_keyboard_event(const KeyEvent& event) {
    _test_text_control.handle_event(event);
    std::cout << _test_text_control.text() << std::endl;
    for (unsigned int i = 0; i < _test_text_control.cursor(); ++i) {
        std::cout << " ";
    }
    std::cout << "^" << std::endl;
}

void Game::update(float dt) {
    for (unsigned int y = 0; y < GRID_HEIGHT; ++y) {
        for (unsigned int x = 0; x < GRID_WIDTH; ++x) {
            _grid_view.set_tile(x, y, rand() % 256, Color(rand() % 256, rand() % 256, rand() % 256, 255), Color(rand() % 256, rand() % 256, rand() % 256, 255), rand() % 2, rand() % 2);
        }
    }
    _grid_view.draw_text(0, 0, _test_text_control.text(), Color(255,255,255), Color(0,0,0));
    
    // Show FPS in the top left corner
    std::stringstream fps_ss;
    unsigned int fps = 1.0f / dt;
    fps_ss << fps;
    _grid_view.draw_text(0, GRID_HEIGHT-1, fps_ss.str(), Color(255,255,255));
}

const GridView<Game::GRID_WIDTH, Game::GRID_HEIGHT>& Game::grid_view() const {
    return _grid_view;
}
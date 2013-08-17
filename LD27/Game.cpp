//
//  Game.cpp
//  LD27
//
//  Created by Kristof Niederholtmeyer on 17.08.13.
//  Copyright (c) 2013 Kristof Niederholtmeyer. All rights reserved.
//

#include "Game.h"

void Game::update(float dt) {
    for (unsigned int y = 0; y < GRID_HEIGHT; ++y) {
        for (unsigned int x = 0; x < GRID_WIDTH; ++x) {
            _grid_view.set_tile(x, y, rand() % 256, Color(rand() % 256, rand() % 256, rand() % 256, 255), Color(rand() % 256, rand() % 256, rand() % 256, 255), rand() % 2, rand() % 2);
        }
    }
}

const GridView<Game::GRID_WIDTH, Game::GRID_HEIGHT>& Game::grid_view() const {
    return _grid_view;
}
//
//  Game.h
//  LD27
//
//  Created by Kristof Niederholtmeyer on 17.08.13.
//  Copyright (c) 2013 Kristof Niederholtmeyer. All rights reserved.
//

#ifndef __LD27__Game__
#define __LD27__Game__

#include <iostream>

#include "Graphics.h"
#include "Input.h"

class Game {
public:
    enum { GRID_WIDTH = 24 };
    enum { GRID_HEIGHT = 24 };
    
private:
    GridView<GRID_WIDTH, GRID_HEIGHT> _grid_view;
    
    TextControl _test_text_control;
    
public:
    Game();
    
    void handle_keyboard_event(const KeyEvent& event);
    void update(float dt);
    const GridView<GRID_WIDTH, GRID_HEIGHT>& grid_view() const;
};

#endif /* defined(__LD27__Game__) */

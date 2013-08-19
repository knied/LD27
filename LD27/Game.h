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
#include "Sound.h"

class Game {
public:
    enum { GRID_WIDTH = 32 };
    enum { GRID_HEIGHT = 24 };
    
private:
    GridView<GRID_WIDTH, GRID_HEIGHT> _grid_view;
    
    TextControl _test_text_control;
    float _cursor_timer;
    Sound _flush_sound;
    
public:
    Game();
    
    void handle_text_event(const std::string& text);
    void handle_keyboard_event(const KeyEvent& event);
    void update(float dt);
    const GridView<GRID_WIDTH, GRID_HEIGHT>& grid_view() const;
};

#endif /* defined(__LD27__Game__) */

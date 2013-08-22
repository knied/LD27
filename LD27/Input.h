//
//  Input.h
//  LD27
//
//  Created by Kristof Niederholtmeyer on 17.08.13.
//  Copyright (c) 2013 Kristof Niederholtmeyer. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __LD27__Input__
#define __LD27__Input__

////////////////////////////////////////////////////////////////////////////////

#include <iostream>

////////////////////////////////////////////////////////////////////////////////

enum Key {
    KEY_NONE = 0x00,
    
    KEY_SPACE = 0x20,
    
    KEY_0 = 0x30,
    KEY_1 = 0x31,
    KEY_2 = 0x32,
    KEY_3 = 0x33,
    KEY_4 = 0x34,
    KEY_5 = 0x35,
    KEY_6 = 0x36,
    KEY_7 = 0x37,
    KEY_8 = 0x38,
    KEY_9 = 0x39,
    
    KEY_A = 0x41,
    KEY_B = 0x42,
    KEY_C = 0x43,
    KEY_D = 0x44,
    KEY_E = 0x45,
    KEY_F = 0x46,
    KEY_G = 0x47,
    KEY_H = 0x48,
    KEY_I = 0x49,
    KEY_J = 0x4A,
    KEY_K = 0x4B,
    KEY_L = 0x4C,
    KEY_M = 0x4D,
    KEY_N = 0x4E,
    KEY_O = 0x4F,
    KEY_P = 0x50,
    KEY_Q = 0x51,
    KEY_R = 0x52,
    KEY_S = 0x53,
    KEY_T = 0x54,
    KEY_U = 0x55,
    KEY_V = 0x56,
    KEY_W = 0x57,
    KEY_X = 0x58,
    KEY_Y = 0x59,
    KEY_Z = 0x5A,
    
    KEY_ARROW_UP = 0x80,
    KEY_ARROW_DOWN = 0x81,
    KEY_ARROW_LEFT = 0x82,
    KEY_ARROW_RIGHT = 0x83,
    
    KEY_BACKSPACE = 0x90,
    KEY_RETURN = 0x91,
    KEY_ESCAPE = 0x91
};

////////////////////////////////////////////////////////////////////////////////

enum KeyEventType {
    KEY_PRESS_EVENT,
    KEY_RELEASE_EVENT
};

////////////////////////////////////////////////////////////////////////////////

struct KeyEvent {
    Key key;
    KeyEventType type;
    
    KeyEvent(Key key, KeyEventType type) : key(key), type(type) {}
};

////////////////////////////////////////////////////////////////////////////////

class Control {
    Key _key[2];
    bool _down[2];
    bool _pressed;
    
public:
    Control(Key key0 = KEY_NONE, Key key1 = KEY_NONE);
    
    void handle_event(const KeyEvent& event);
    
    // True, if one of the keys is currently held down.
    bool down() const;
    // True, if one of the keys has been pressed since the last clear.
    bool pressed() const;
    // Clears the pressed flag.
    void clear();
    
};

////////////////////////////////////////////////////////////////////////////////

class TextControl {
    std::string _text;
    unsigned int _cursor;
    bool _support_A_to_Z;
    bool _support_0_to_9;
    bool _support_space;
    std::string _support_custom;
    
    bool character_is_supported(char character) {
        if (_support_0_to_9 && character >= '0' && character <= '9') {
            return true;
        }
        if (_support_A_to_Z && character >= 'A' && character <= 'Z') {
            return true;
        }
        if (_support_space && character == ' ') {
            return true;
        }
        for (unsigned int i = 0; i < _support_custom.length(); ++i) {
            if (character == _support_custom[i]) {
                return true;
            }
        }
        return false;
    }
    
public:
    TextControl(const std::string& text = "",
                bool support_A_to_Z = true,
                bool support_0_to_9 = true,
                bool support_space = true,
                const std::string& support_custom = ".()[]{}+-*/=:");
    
    void handle_text_event(const std::string& text);
    void handle_event(const KeyEvent& event);
    
    void set_text(const std::string& text);
    
    const std::string& text() const;
    unsigned int cursor() const;
};

////////////////////////////////////////////////////////////////////////////////

#endif /* defined(__LD27__Input__) */

////////////////////////////////////////////////////////////////////////////////

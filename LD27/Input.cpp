//
//  Input.cpp
//  LD27
//
//  Created by Kristof Niederholtmeyer on 17.08.13.
//  Copyright (c) 2013 Kristof Niederholtmeyer. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "Input.h"

////////////////////////////////////////////////////////////////////////////////

Control::Control(Key key0, Key key1) {
    _key[0] = key0;
    _key[1] = key1;
}

void Control::handle_event(const KeyEvent& event) {
    if (event.key == _key[0]) {
        if (event.type == KEY_PRESS_EVENT) {
            if (!down()) {
                _pressed = true;
            }
            _down[0] = true;
        } else {
            _down[0] = false;
        }
    }
    if (event.key == _key[1]) {
        if (event.type == KEY_PRESS_EVENT) {
            if (!down()) {
                _pressed = true;
            }
            _down[1] = true;
        } else {
            _down[1] = false;
        }
    }
}

bool Control::down() const {
    return _down[0] || _down[1];
}

bool Control::pressed() const {
    return _pressed;
}

void Control::clear() {
    _pressed = false;
}

////////////////////////////////////////////////////////////////////////////////

TextControl::TextControl(const std::string& text,
            bool support_A_to_Z,
            bool support_0_to_9,
            bool support_space,
            const std::string& support_custom)
: _text(text), _cursor((unsigned int)text.length()), _support_A_to_Z(support_A_to_Z), _support_0_to_9(support_0_to_9), _support_space(support_space), _support_custom(support_custom) {

}

void TextControl::handle_text_event(const std::string& text) {
    if (text.length() > 0) {
        unsigned char character = text[0];
        if (character >= 'a' && character <= 'z') {
            character = character - 'a' + 'A';
        }
        
        if (character_is_supported(character)) {
            _text.insert(_text.begin() + _cursor, character);
            _cursor++;
        }
    }
}

void TextControl::handle_event(const KeyEvent& event) {
    if (event.type == KEY_PRESS_EVENT) {
        if (event.key == KEY_BACKSPACE) {
            if (_cursor > 0) {
                _text.erase(_text.begin() + (_cursor - 1));
                _cursor--;
            }
        } else if (event.key == KEY_ARROW_LEFT) {
            if (_cursor > 0) {
                _cursor--;
            }
        } else if (event.key == KEY_ARROW_RIGHT) {
            if (_cursor < _text.length()) {
                _cursor++;
            }
        }
    }
}

void TextControl::set_text(const std::string& text) {
    _text = text;
    _cursor = (unsigned int)_text.length();
}

const std::string& TextControl::text() const {
    return _text;
}

unsigned int TextControl::cursor() const {
    return _cursor;
}

////////////////////////////////////////////////////////////////////////////////
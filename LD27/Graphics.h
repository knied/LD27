//
//  Graphics.h
//  LD27
//
//  Created by Kristof Niederholtmeyer on 17.08.13.
//  Copyright (c) 2013 Kristof Niederholtmeyer. All rights reserved.
//

#ifndef __LD27__Graphics__
#define __LD27__Graphics__

#include <string>

struct Color {
    unsigned char r, g, b, a;
    
    Color() : r(0), g(0), b(0), a(255) {}
    Color(unsigned char r,
          unsigned char g,
          unsigned char b,
          unsigned char a = 255)
    : r(r), g(g), b(b), a(a) {}
};

struct Tile {
    Color symbol_color;
    Color background_color;
    unsigned char symbol;
    bool flip_x, flip_y;
    
    Tile() : symbol(0), flip_x(false), flip_y(false) {}
    Tile(unsigned char symbol,
         const Color& symbol_color,
         const Color& background_color,
         bool flip_x = false,
         bool flip_y = false)
    : symbol_color(symbol_color), background_color(background_color), symbol(symbol), flip_x(flip_x), flip_y(flip_y) {}
};

template <unsigned int GRID_WIDTH, unsigned int GRID_HEIGHT>
class GridView {
    Tile _tiles[GRID_WIDTH * GRID_HEIGHT];
    
public:
    void set_tile(unsigned int x,
                  unsigned int y,
                  const Tile& tile) {
        _tiles[y * GRID_WIDTH + x] = tile;
    }
    void set_tile(unsigned int x,
                  unsigned int y,
                  unsigned char symbol,
                  const Color& symbol_color,
                  const Color& background_color,
                  bool flip_x = false,
                  bool flip_y = false) {
        _tiles[y * GRID_WIDTH + x] = Tile(symbol, symbol_color, background_color, flip_x, flip_y);
    }
    void set_symbol(unsigned int x, unsigned int y, unsigned char symbol) {
        _tiles[y * GRID_WIDTH + x].symbol = symbol;
    }
    void set_symbol_color(unsigned int x, unsigned int y, const Color& symbol_color) {
        _tiles[y * GRID_WIDTH + x].symbol_color = symbol_color;
    }
    void set_background_color(unsigned int x, unsigned int y, const Color& background_color) {
        _tiles[y * GRID_WIDTH + x].background_color = background_color;
    }
    void set_flip(unsigned int x, unsigned int y, bool flip_x, bool flip_y) {
        _tiles[y * GRID_WIDTH + x].flip_x = flip_x;
        _tiles[y * GRID_WIDTH + x].flip_y = flip_y;
    }
    
    void draw_character(unsigned int x, unsigned int y, char character) {
        set_flip(x, y, false, false);
        if (character >= '1' && character <= '9') {
            set_symbol(x, y, character - '1' + 208);
        } else if (character == '0') {
            set_symbol(x, y, 217);
        } else if (character >= 'A' && character <= 'Z') {
            set_symbol(x, y, character - 'A' + 224);
        } else {
            set_symbol(x, y, 223);
        }
    }
    void draw_text(unsigned int x, unsigned int y, const std::string& text, const Color& text_color) {
        unsigned int index = 0;
        while (x < GRID_WIDTH && y < GRID_HEIGHT && index < text.length()) {
            char character = text[index];
            set_symbol_color(x, y, text_color);
            draw_character(x, y, character);
            
            x++;
            index++;
        }
    }
    void draw_text(unsigned int x, unsigned int y, const std::string& text, const Color& text_color, const Color& background_color) {
        unsigned int index = 0;
        while (x < GRID_WIDTH && y < GRID_HEIGHT && index < text.length()) {
            char character = text[index];
            set_symbol_color(x, y, text_color);
            set_background_color(x, y, background_color);
            draw_character(x, y, character);
            
            x++;
            index++;
        }
    }
    void set_background_color_in_rect(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, const Color& background_color) {
        if (x1 >= GRID_WIDTH) {
            x1 = GRID_WIDTH-1;
        }
        if (y1 >= GRID_HEIGHT) {
            y1 = GRID_HEIGHT-1;
        }
        if (x0 <= x1 && y0 <= y1) {
            for (unsigned int y = y0; y <= y1; ++y) {
                for (unsigned int x = x0; x <= x1; ++x) {
                    set_background_color(x, y, background_color);
                }
            }
        }
    }
    
    const Tile& tile(unsigned int x, unsigned int y) const {
        return _tiles[y * GRID_WIDTH + x];
    }
    unsigned char symbol(unsigned int x, unsigned int y) const {
        return _tiles[y * GRID_WIDTH + x].symbol;
    }
    const Color& symbol_color(unsigned int x, unsigned int y) const {
        return _tiles[y * GRID_WIDTH + x].symbol_color;
    }
    const Color& background_color(unsigned int x, unsigned int y) const {
        return _tiles[y * GRID_WIDTH + x].background_color;
    }
    bool flip_x(unsigned int x, unsigned int y) const {
        return _tiles[y * GRID_WIDTH + x].flip_x;
    }
    bool flip_y(unsigned int x, unsigned int y) const {
        return _tiles[y * GRID_WIDTH + x].flip_y;
    }
};

#endif /* defined(__LD27__Graphics__) */

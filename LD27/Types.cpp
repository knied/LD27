//
//  Types.cpp
//  LD27
//
//  Created by Kristof Niederholtmeyer on 24.08.13.
//  Copyright (c) 2013 Kristof Niederholtmeyer. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "Types.h"
#include <math.h>

////////////////////////////////////////////////////////////////////////////////

struct Transformation {
    int _m00; int _m01;
    int _m10; int _m11;
    
    int x(const int x, const int y) const {return _m00 * x + _m01 * y;}
    int y(const int x, const int y) const {return _m10 * x + _m11 * y;}
    
    Transformation() : _m00(1), _m01(0), _m10(0), _m11(1) {}
    Transformation(int m00, int m01, int m10, int m11) : _m00(m00), _m01(m01), _m10(m10), _m11(m11) {}
};

static void bresenham(int x0, int y0, int x1, int y1,
                      const Transformation& transformation,
                      std::vector<Position>& points) {
    int a = y1 - y0;
    int b = x0 - x1;
    float m = (float)(y1 - y0) / (float)(x1 - x0);
    float t = (float)y0 - m * (float)x0;
    int c = roundf((float)(x1 - x0) * t);
    int dE = 2 * a;
    int dNE = 2 * (a + b);
    int d = 2 * a * (x0 + 1) + b * (2 * y0 + 1) + 2 * c;
    int current_x = x0;
    int current_y = y0;
    unsigned int index = 1;
    while (current_x < x1 || current_y < y1) {
        if (d > 0) {
            // NE
            d += dNE;
            current_x++;
            current_y++;
        } else {
            // E
            d += dE;
            current_x++;
        }
        
        //std::cout << index << " " << transformation.x(current_x, current_y) << " " << transformation.y(current_x, current_y) <<  std::endl;
        points[index++] = Position(transformation.x(current_x, current_y),
                                   transformation.y(current_x, current_y));
    }
}

void Line::update_points() {
    unsigned int size = maximum(abs(_end.x - _start.x), abs(_end.y - _start.y));
    _points.resize(size+1);
    _points[0] = _start;
    
    int from_x = _start.x;
    int from_y = _start.y;
    int x = _end.x;
    int y = _end.y;

    int dx = x - from_x;
    int dy = y - from_y;
    
    if (dx == 0) {
        // special case to prevent division by zero
        int current_x = from_x;
        int current_y = from_y;
        if (dy > 0) {
            unsigned int index = 1;
            current_y++;
            for (; current_y <= y; ++current_y) {
                _points[index++] = Position(current_x, current_y);
            }
        } else if (dy < 0) {
            unsigned int index = 1;
            current_y--;
            for (; current_y >= y; --current_y) {
                _points[index++] = Position(current_x, current_y);
            }
        }
    } else if (dx > 0 && dy >= 0) {
        if ((float)dy / (float)dx < 1.0f) {
            /*
             //       ^y
             //   \   |   /
             //     \ | / X
             // ------|------>x
             //     / | \
             //   /   |   \
             */
            bresenham(from_x, from_y, x, y, Transformation(1,0,0,1), _points);
        } else {
            /*
             //       ^y
             //   \   | X /
             //     \ | /
             // ------|------>x
             //     / | \
             //   /   |   \
             */
            bresenham(from_y, from_x, y, x, Transformation(0,1,1,0), _points);
        }
    } else if (dx < 0 && dy >= 0) {
        if ((float)dy / (float)dx < -1.0f) {
            /*
             //       ^y
             //   \   |   /
             //   X \ | /
             // ------|------>x
             //     / | \
             //   /   |   \
             */
            bresenham(from_y, -from_x, y, -x, Transformation(0,-1,1,0), _points);
        } else {
            /*
             //       ^y
             //   \ X |   /
             //     \ | /
             // ------|------>x
             //     / | \
             //   /   |   \
             */
            bresenham(-from_x, from_y, -x, y, Transformation(-1,0,0,1), _points);
        }
    } else if (dx > 0 && dy < 0) {
        if ((float)dy / (float)dx >= -1.0f) {
            /*
             //       ^y
             //   \   |   /
             //     \ | /
             // ------|------>x
             //     / | \ X
             //   /   |   \
             */
            bresenham(from_x, -from_y, x, -y, Transformation(1,0,0,-1), _points);
        } else {
            /*
             //       ^y
             //   \   |   /
             //     \ | /
             // ------|------>x
             //     / | \
             //   /   | X \
             */
            bresenham(-from_y, from_x, -y, x, Transformation(0,1,-1,0), _points);
        }
    } else { // dx < 0 && dy < 0
        if ((float)dy / (float)dx > 1.0f) {
            /*
             //       ^y
             //   \   |   /
             //     \ | /
             // ------|------>x
             //     / | \
             //   / X |   \
             */
            bresenham(-from_y, -from_x, -y, -x, Transformation(0,-1,-1,0), _points);
        } else {
            /*
             //       ^y
             //   \   |   /
             //     \ | /
             // ------|------>x
             //   X / | \
             //   /   |   \
             */
            bresenham(-from_x, -from_y, -x, -y, Transformation(-1,0,0,-1), _points);
        }
    }
}

Line::Line(const Position& start, const Position& end)
: _start(start), _end(end) {
    update_points();
}

void Line::set(const Position& start, const Position& end) {
    _start = start;
    _end = end;
    update_points();
}

void Line::set_start(const Position& start) {
    _start = start;
    update_points();
}

void Line::set_end(const Position& end) {
    _end = end;
    update_points();
}

unsigned int Line::size() const {
    return (unsigned int)_points.size();
}

const Position& Line::operator [] (unsigned int index) const {
    return _points[index];
}

const Position& Line::start() const {
    return _start;
}

const Position& Line::end() const {
    return _end;
}

Direction Line::direction() const {
    int dx = _end.x - _start.x;
    int dy = _end.y - _start.y;
    
    if (abs(dx) > abs(dy)) {
        if (dx < 0) {
            return WEST;
        } else {
            return EAST;
        }
    } else {
        if (dy < 0) {
            return SOUTH;
        } else {
            return NORTH;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
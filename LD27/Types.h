//
//  Types.h
//  LD27
//
//  Created by Kristof Niederholtmeyer on 24.08.13.
//  Copyright (c) 2013 Kristof Niederholtmeyer. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __LD27__Types__
#define __LD27__Types__

////////////////////////////////////////////////////////////////////////////////

#define PI 3.14159265359

////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>

////////////////////////////////////////////////////////////////////////////////

template <typename Type>
Type minimum(const Type& a, const Type& b) {
    return a < b ? a : b;
}

template <typename Type>
Type maximum(const Type& a, const Type& b) {
    return a > b ? a : b;
}

struct Position {
    int x, y;
    int old_x, old_y;
    
    Position() : x(0), y(0) {}
    Position(int x, int y) : x(x), y(y), old_x(x), old_y(y) {}
    const Position& operator = (const Position& position) {
        old_x = x;
        old_y = y;
        x = position.x;
        y = position.y;
        return *this;
    }
};

typedef enum {
    NORTH,
    SOUTH,
    EAST,
    WEST
} Direction;

struct Orientation {
    Position position;
    Direction direction;
    
    Orientation() : direction(NORTH) {}
    Orientation(const Position& position, const Direction& direction = NORTH)
    : position(position), direction(direction) {}
};

class Line {
    Position _start;
    Position _end;
    std::vector<Position> _points;
    
    void update_points();
    
public:
    Line(const Position& start = Position(), const Position& end = Position());
    
    void set(const Position& start, const Position& end);
    void set_start(const Position& start);
    void set_end(const Position& end);
    
    unsigned int size() const;
    const Position& operator [] (unsigned int index) const;
    
    const Position& start() const;
    const Position& end() const;
    
    Direction direction() const;
};

////////////////////////////////////////////////////////////////////////////////

#endif /* defined(__LD27__Types__) */

////////////////////////////////////////////////////////////////////////////////

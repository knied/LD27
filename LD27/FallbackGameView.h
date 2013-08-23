//
//  FallbackGameView.h
//  LD27
//
//  Created by Kristof Niederholtmeyer on 23.08.13.
//  Copyright (c) 2013 Kristof Niederholtmeyer. All rights reserved.
//

#ifndef __LD27__FallbackGameView__
#define __LD27__FallbackGameView__

#include <iostream>
#include <sys/time.h>
#include <QWidget>
#include <QPainter>
#include <QBitmap>

#include "Game.h"

class FallbackGameView : public QWidget {
public:
    enum { GRID_WIDTH = 24 };
    enum { GRID_HEIGHT = 24 };
    enum { TILE_WIDTH = 24 };
    enum { TILE_HEIGHT = 24 };
    enum { VIEW_WIDTH = GRID_WIDTH * TILE_WIDTH };
    enum { VIEW_HEIGHT = GRID_HEIGHT * TILE_HEIGHT };
    
private:
    QBrush _tilemap_brush;
    QBrush _fill_brush;
    
    
public:
    FallbackGameView(QWidget *parent = 0);
    
    void paintEvent(QPaintEvent* event);
    void showEvent(QShowEvent* event);
    void timerEvent(QTimerEvent* event);
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
private:
    void move();
    void handle_keyboard_event(QKeyEvent* event, KeyEventType type);
    
    Game _game;
    
    timeval _lastTime;
    float _time_accumulator;
    
    int _timer_id;
    
};

#endif /* defined(__LD27__FallbackGameView__) */

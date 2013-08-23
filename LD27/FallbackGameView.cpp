//
//  FallbackGameView.cpp
//  LD27
//
//  Created by Kristof Niederholtmeyer on 23.08.13.
//  Copyright (c) 2013 Kristof Niederholtmeyer. All rights reserved.
//

#include "FallbackGameView.h"

FallbackGameView::FallbackGameView(QWidget *parent) : QWidget(parent), _time_accumulator(0.0f), _timer_id(0) {
    setFocusPolicy(Qt::StrongFocus);
    
    QPixmap mask(":/8x8tiles.png");
    mask = mask.scaled(TILE_WIDTH * 16, TILE_HEIGHT * 16);
    _tilemap_brush = QBrush(QColor(255,0,0,255), mask.createMaskFromColor(QColor(0,0,0,0)));
    _tilemap_brush.setTransform(QTransform::fromTranslate(2*TILE_WIDTH, 2*TILE_HEIGHT));
    _fill_brush = QBrush(Qt::SolidPattern);
}

void FallbackGameView::move() {
    //std::cout << "move" << std::endl;
    
    timeval tmpTime = _lastTime;
    gettimeofday(&_lastTime, 0);
    float frame_time = (float)(_lastTime.tv_sec - tmpTime.tv_sec) + (float)(_lastTime.tv_usec - tmpTime.tv_usec) * 0.000001f;
    const float dt = 0.05f;
    
    //std::cout << "FPS: " << 1.0f / frame_time << std::endl;
    
    _time_accumulator += frame_time;
    
    bool needs_redraw = false;
    while (_time_accumulator >= dt) {
        _game.update(dt);
        _time_accumulator -= dt;
        needs_redraw = true;
    }
    
    if (needs_redraw) {
        update();
        //repaint();
    }
}

void FallbackGameView::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.setPen(Qt::NoPen);
    p.setBrush(_tilemap_brush);
    p.setBackgroundMode(Qt::OpaqueMode);
    
    //std::cout << "draw" << std::endl;
    
    for(int y = 0; y < GRID_WIDTH; ++y) {
        for(int x = 0; x < GRID_HEIGHT; ++x) {
            Tile tile = _game.grid_view().tile(x, y);
            
            int tile_x = tile.symbol % 16;
            int tile_y = tile.symbol / 16;
            
            QTransform scale = QTransform::fromScale(tile.flip_x ? -1 : 1, tile.flip_y ? -1 : 1);
            int trans_x = (x + (tile.flip_x ? (tile_x+1) : -tile_x)) * TILE_WIDTH;
            int trans_y = ((GRID_HEIGHT - y - 1) + (tile.flip_y ? (tile_y+1) : -tile_y)) * TILE_HEIGHT;
            QTransform translation = QTransform::fromTranslate(trans_x,trans_y);
            _tilemap_brush.setTransform(scale * translation);
            
            Color symbol_color = tile.symbol_color;
            Color background_color = tile.background_color;
            _tilemap_brush.setColor(QColor(symbol_color.r,
                                           symbol_color.g,
                                           symbol_color.b,
                                           symbol_color.a));
            _fill_brush.setColor(QColor(background_color.r,
                                        background_color.g,
                                        background_color.b,
                                        background_color.a));
            p.setBrush(_tilemap_brush);
            p.setBackground(_fill_brush);
            p.drawRect(x * TILE_WIDTH,
                       (GRID_HEIGHT - y - 1) * TILE_HEIGHT,
                       TILE_WIDTH, TILE_HEIGHT);
        }
    }
}

void FallbackGameView::showEvent(QShowEvent * /* event */)
{
    _timer_id = startTimer(50);
    gettimeofday(&_lastTime, 0);
}

void FallbackGameView::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == _timer_id)
    {
        move();
    }
    else {
        QWidget::timerEvent(event);
    }
}

void FallbackGameView::keyPressEvent(QKeyEvent *event)
{
    handle_keyboard_event(event, KEY_PRESS_EVENT);
}

void FallbackGameView::keyReleaseEvent(QKeyEvent *event) {
    handle_keyboard_event(event, KEY_RELEASE_EVENT);
}

void FallbackGameView::handle_keyboard_event(QKeyEvent* event, KeyEventType type) {
    if (type == KEY_PRESS_EVENT) {
        std::string text = event->text().toStdString();
        //std::cout << text << std::endl;
        _game.handle_text_event(text);
    }
    
    Key key = KEY_NONE;
    
    switch (event->key()) {
        case Qt::Key_Space: key = KEY_SPACE; break;
            
        case Qt::Key_0: key = KEY_0; break;
        case Qt::Key_1: key = KEY_1; break;
        case Qt::Key_2: key = KEY_2; break;
        case Qt::Key_3: key = KEY_3; break;
        case Qt::Key_4: key = KEY_4; break;
        case Qt::Key_5: key = KEY_5; break;
        case Qt::Key_6: key = KEY_6; break;
        case Qt::Key_7: key = KEY_7; break;
        case Qt::Key_8: key = KEY_8; break;
        case Qt::Key_9: key = KEY_9; break;
            
        case Qt::Key_A: key = KEY_A; break;
        case Qt::Key_B: key = KEY_B; break;
        case Qt::Key_C: key = KEY_C; break;
        case Qt::Key_D: key = KEY_D; break;
        case Qt::Key_E: key = KEY_E; break;
        case Qt::Key_F: key = KEY_F; break;
        case Qt::Key_G: key = KEY_G; break;
        case Qt::Key_H: key = KEY_H; break;
        case Qt::Key_I: key = KEY_I; break;
        case Qt::Key_J: key = KEY_J; break;
        case Qt::Key_K: key = KEY_K; break;
        case Qt::Key_L: key = KEY_L; break;
        case Qt::Key_M: key = KEY_M; break;
        case Qt::Key_N: key = KEY_N; break;
        case Qt::Key_O: key = KEY_O; break;
        case Qt::Key_P: key = KEY_P; break;
        case Qt::Key_Q: key = KEY_Q; break;
        case Qt::Key_R: key = KEY_R; break;
        case Qt::Key_S: key = KEY_S; break;
        case Qt::Key_T: key = KEY_T; break;
        case Qt::Key_U: key = KEY_U; break;
        case Qt::Key_V: key = KEY_V; break;
        case Qt::Key_W: key = KEY_W; break;
        case Qt::Key_X: key = KEY_X; break;
        case Qt::Key_Y: key = KEY_Y; break;
        case Qt::Key_Z: key = KEY_Z; break;
            
        case Qt::Key_Up: key = KEY_ARROW_UP; break;
        case Qt::Key_Down: key = KEY_ARROW_DOWN; break;
        case Qt::Key_Left: key = KEY_ARROW_LEFT; break;
        case Qt::Key_Right: key = KEY_ARROW_RIGHT; break;
            
        case Qt::Key_Backspace: key = KEY_BACKSPACE; break;
        case Qt::Key_Return: case Qt::Key_Enter: key = KEY_RETURN; break;
        case Qt::Key_Escape: key = KEY_ESCAPE; break;
            
        default: QWidget::keyPressEvent(event); break;
    }
    
    if (key != KEY_NONE) {
        _game.handle_keyboard_event(KeyEvent(key, type));
    }
}
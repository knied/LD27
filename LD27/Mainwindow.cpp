//
//  MainWindow.cpp
//  LD27
//
//  Created by Kristof Niederholtmeyer on 17.08.13.
//  Copyright (c) 2013 Kristof Niederholtmeyer. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "Mainwindow.h"
#include <iostream>

////////////////////////////////////////////////////////////////////////////////

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
#ifdef USE_OPENGL
    QGLFormat format;
#ifdef USE_OPENGL_3_2
    format.setVersion(3,2);
#else
    format.setVersion(2,1);
#endif
    format.setProfile(QGLFormat::CoreProfile);
    _game_view = 0;
    _game_view = new GameView(format);
    if (_game_view)
        setCentralWidget(_game_view);
    setFixedSize(GameView::VIEW_WIDTH,
                 GameView::VIEW_HEIGHT);
#else
    _game_view = 0;
    _game_view = new FallbackGameView();
    if (_game_view) {
        setCentralWidget(_game_view);
    }
    setFixedSize(FallbackGameView::VIEW_WIDTH,
                 FallbackGameView::VIEW_HEIGHT);
#endif
}

MainWindow::~MainWindow()
{
    delete _game_view;
}

////////////////////////////////////////////////////////////////////////////////

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
    QGLFormat format;
    format.setVersion(3,2);
    format.setProfile(QGLFormat::CoreProfile);
    _game_view = 0;
    _game_view = new GameView(format);
    if (_game_view)
        setCentralWidget(_game_view);
    setFixedSize(GameView::VIEW_WIDTH,
                 GameView::VIEW_HEIGHT);
}

MainWindow::~MainWindow()
{
    delete _game_view;
}

////////////////////////////////////////////////////////////////////////////////

//
//  Gameview.cpp
//  LD27
//
//  Created by Kristof Niederholtmeyer on 17.08.13.
//  Copyright (c) 2013 Kristof Niederholtmeyer. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "Gameview.h"

#include <QtGui>
#include <iostream>
#include <vector>

////////////////////////////////////////////////////////////////////////////////

struct Vertex {
    float x, y, z;
    float u, v;
    Color symbol_color;
    Color background_color;
};

////////////////////////////////////////////////////////////////////////////////

GameView::GameView(const QGLFormat& format, QWidget *parent)
: QGLWidget(format, parent), _time_accumulator(0.0f), _timer_id(0)
{
    setFocusPolicy(Qt::StrongFocus);
}

void GameView::compile_shader(GLuint shader_identifier, const std::string& shader_source) {
    GLint shader_ok = 0;

    // compile the vertex shader
    const char* c_source = shader_source.c_str();
    glShaderSource(shader_identifier, 1, &c_source, 0);
    glCompileShader(shader_identifier);
    glGetShaderiv(shader_identifier, GL_COMPILE_STATUS, &shader_ok);
    if (!shader_ok) {
        GLint log_length;
        glGetShaderiv(shader_identifier, GL_INFO_LOG_LENGTH, &log_length);
        char *log = new char[log_length];
        glGetShaderInfoLog(shader_identifier, log_length, 0, log);
        std::cout << shader_source << std::endl;
        std::cout << "ERROR: Unable to compile shader:" << std::endl;
        std::cout << log << std::endl;
        delete [] log;
    }
}

void GameView::link_program() {
    glLinkProgram(_program_identifier);
    GLint program_ok;
    glGetProgramiv(_program_identifier, GL_LINK_STATUS, &program_ok);
    if (!program_ok) {
        GLint log_length;
        glGetProgramiv(_program_identifier, GL_INFO_LOG_LENGTH, &log_length);
        char* log = new char[log_length];
        glGetProgramInfoLog(_program_identifier, log_length, 0, log);
        std::cout << "ERROR: Unable to link program" << std::endl;
        std::cout << log << std::endl;
        delete [] log;
    }
}

void GameView::update_vertex_buffer() {
    Vertex vertex_data[GRID_WIDTH * GRID_HEIGHT * 6];
    for(int y = 0; y < GRID_HEIGHT; ++y) {
        for(int x = 0; x < GRID_WIDTH; ++x) {
            Tile tile = _game.grid_view().tile(x, y);
            
            float tmp = 1.0f / 16.0f;
            
            int tile_x = tile.symbol % 16;
            int tile_y = tile.symbol / 16;

            float u0 = 0.0f;
            float u1 = 0.0f;
            float v0 = 0.0f;
            float v1 = 0.0f;
            if (tile.flip_x) {
                u1 = tile_x * tmp;
                u0 = (tile_x + 1) * tmp;
            } else {
                u0 = tile_x * tmp;
                u1 = (tile_x + 1) * tmp;
            }
            if (tile.flip_y) {
                v1 = 1.0f - (tile_y) * tmp;
                v0 = 1.0f - (tile_y + 1) * tmp;
            } else {
                v0 = 1.0f - (tile_y) * tmp;
                v1 = 1.0f - (tile_y + 1) * tmp;
            }
            
            Color symbol_color = tile.symbol_color;
            Color background_color = tile.background_color;

            float x_offset = -(float)GRID_WIDTH / 2.0f * (float)TILE_WIDTH;
            float y_offset = -(float)GRID_HEIGHT / 2.0f * (float)TILE_HEIGHT;

            Vertex vertex0 = {(float)(x) * (float)TILE_WIDTH + x_offset, (float)(y+1) * (float)TILE_HEIGHT + y_offset, 0.0f, u0, v0, symbol_color, background_color};
            Vertex vertex1 = {(float)(x+1) * (float)TILE_WIDTH + x_offset, (float)(y+1) * (float)TILE_HEIGHT + y_offset, 0.0f, u1, v0, symbol_color, background_color};
            Vertex vertex2 = {(float)(x+1) * (float)TILE_WIDTH + x_offset, (float)(y) * (float)TILE_HEIGHT + y_offset, 0.0f, u1, v1, symbol_color, background_color};
            Vertex vertex3 = {(float)(x) * (float)TILE_WIDTH + x_offset, (float)(y) * (float)TILE_HEIGHT + y_offset, 0.0f, u0, v1, symbol_color, background_color};

            vertex_data[6 * GRID_WIDTH * y + 6 * x + 0] = vertex0;
            vertex_data[6 * GRID_WIDTH * y + 6 * x + 1] = vertex1;
            vertex_data[6 * GRID_WIDTH * y + 6 * x + 2] = vertex2;

            vertex_data[6 * GRID_WIDTH * y + 6 * x + 3] = vertex0;
            vertex_data[6 * GRID_WIDTH * y + 6 * x + 4] = vertex2;
            vertex_data[6 * GRID_WIDTH * y + 6 * x + 5] = vertex3;
        }
    }

#ifdef USE_VAO
    glBindVertexArray(_vao);
#endif
    glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer_identifier);
    glBufferData(GL_ARRAY_BUFFER,
                 GRID_WIDTH * GRID_HEIGHT * 6 * sizeof(Vertex),
                 &vertex_data[0],
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_TRUE,
                          sizeof(Vertex),
                          0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,
                          2,
                          GL_FLOAT,
                          GL_TRUE,
                          sizeof(Vertex),
                          &(((Vertex*)0)->u));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,
                          4,
                          GL_UNSIGNED_BYTE,
                          GL_TRUE,
                          sizeof(Vertex),
                          &(((Vertex*)0)->symbol_color));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3,
                          4,
                          GL_UNSIGNED_BYTE,
                          GL_TRUE,
                          sizeof(Vertex),
                          &(((Vertex*)0)->background_color));
}

void GameView::initializeGL()
{
    glClearColor(0,0,0,0);

#ifdef USE_VAO
    glGenVertexArrays(1, &_vao);
#endif
    glGenBuffers(1, &_vertex_buffer_identifier);
    _program_identifier = glCreateProgram();

    QImage img;
    if (!img.load(":/8x8tiles.png")) {
        std::cout << "image not found!" << std::endl;
    }

    QImage GL_formatted_image;
    GL_formatted_image = QGLWidget::convertToGLFormat(img);

    glGenTextures(1, &_texture_identifier);
    glBindTexture(GL_TEXTURE_2D, _texture_identifier);
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, GL_formatted_image.width(),
                  GL_formatted_image.height(),
                  0, GL_RGBA, GL_UNSIGNED_BYTE, GL_formatted_image.bits() );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    update_vertex_buffer();

#ifdef USE_GLSL_150
    // Use GLSL 1.5
    std::string vertex_shader;
    vertex_shader += "#version 150\n";

    vertex_shader += "in vec4 vertex;\n";
    vertex_shader += "in vec2 texcoord;\n";
    vertex_shader += "in vec4 symbol_color;\n";
    vertex_shader += "in vec4 background_color;\n";

    vertex_shader += "uniform mat4 mvp;\n";

    vertex_shader += "out vec2 v_texcoord;\n";
    vertex_shader += "out vec4 v_symbol_color;\n";
    vertex_shader += "out vec4 v_background_color;\n";

    vertex_shader += "void main() {\n";
    vertex_shader += "  gl_Position = mvp * vertex;\n";
    vertex_shader += "  v_texcoord = texcoord;\n";
    vertex_shader += "  v_symbol_color = symbol_color;\n";
    vertex_shader += "  v_background_color = background_color;\n";
    vertex_shader += "}\n";

    std::string fragment_shader;
    fragment_shader += "#version 150\n";

    fragment_shader += "in vec2 v_texcoord;\n";
    fragment_shader += "in vec4 v_symbol_color;\n";
    fragment_shader += "in vec4 v_background_color;\n";

    fragment_shader += "uniform sampler2D texture0;\n";

    fragment_shader += "out vec4 frag_color;\n";

    fragment_shader += "void main() {\n";
    fragment_shader += "  float r = texture(texture0, v_texcoord).r;\n";
    fragment_shader += "  frag_color = v_symbol_color * r + v_background_color * (1.0 - r);\n";
    fragment_shader += "}\n";
#else
    // Use GLSL 1.2
    std::string vertex_shader;
    vertex_shader += "#version 120\n";

    vertex_shader += "attribute vec4 vertex;\n";
    vertex_shader += "attribute vec2 texcoord;\n";
    vertex_shader += "attribute vec4 symbol_color;\n";
    vertex_shader += "attribute vec4 background_color;\n";

    vertex_shader += "uniform mat4 mvp;\n";

    vertex_shader += "varying vec2 v_texcoord;\n";
    vertex_shader += "varying vec4 v_symbol_color;\n";
    vertex_shader += "varying vec4 v_background_color;\n";

    vertex_shader += "void main() {\n";
    vertex_shader += "  gl_Position = mvp * vertex;\n";
    vertex_shader += "  v_texcoord = texcoord;\n";
    vertex_shader += "  v_symbol_color = symbol_color;\n";
    vertex_shader += "  v_background_color = background_color;\n";
    vertex_shader += "}\n";

    std::string fragment_shader;
    fragment_shader += "#version 120\n";

    fragment_shader += "varying vec2 v_texcoord;\n";
    fragment_shader += "varying vec4 v_symbol_color;\n";
    fragment_shader += "varying vec4 v_background_color;\n";

    fragment_shader += "uniform sampler2D texture0;\n";

    fragment_shader += "void main() {\n";
    fragment_shader += "  float r = texture2D(texture0, v_texcoord).r;\n";
    fragment_shader += "  gl_FragColor = v_symbol_color * r + v_background_color * (1.0 - r);\n";
    fragment_shader += "}\n";
#endif

    GLuint vertex_shader_identifier = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment_shader_identifier = glCreateShader(GL_FRAGMENT_SHADER);

    compile_shader(vertex_shader_identifier, vertex_shader);
    compile_shader(fragment_shader_identifier, fragment_shader);

    glAttachShader(_program_identifier, vertex_shader_identifier);
    glAttachShader(_program_identifier, fragment_shader_identifier);

    glBindAttribLocation(_program_identifier, 0, "vertex");
    glBindAttribLocation(_program_identifier, 1, "texcoord");
    glBindAttribLocation(_program_identifier, 2, "symbol_color");
    glBindAttribLocation(_program_identifier, 3, "background_color");

    link_program();

    glDetachShader(_program_identifier, vertex_shader_identifier);
    glDetachShader(_program_identifier, fragment_shader_identifier);

    glDeleteShader(fragment_shader_identifier);
    glDeleteShader(vertex_shader_identifier);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cout << "Error: " << error << std::endl;
    }

    gettimeofday(&_lastTime, 0);
}

void GameView::resizeGL(int width, int height)
{
    glViewport(0,0,width,height);

    glBindTexture(GL_TEXTURE_2D, _texture_identifier);
    glUseProgram(_program_identifier);
    GLuint mvp_position = glGetUniformLocation(_program_identifier, "mvp");
    float znear = -10.0f;
    float zfar = 10.0f;
    float left = -(float)width / 2.0f;
    float right = -left;
    float top = (float)height / 2.0f;
    float bottom = -top;
    float mvp[16] = {
        2.0f / (right -left),    0.0f,                   0.0f,                   -(right+left)/(right-left),
        0.0f,                    2.0f / (top-bottom),    0.0f,                   -(top+bottom)/(top-bottom),
        0.0f,                    0.0f,                   -2.0f / (zfar-znear),   -(zfar+znear)/(zfar-znear),
        0.0f,                    0.0f,                   0.0f,                   1.0f
    };
    glUniformMatrix4fv(mvp_position, 1, GL_FALSE, mvp);
}

void GameView::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(_program_identifier);
#ifdef USE_VAO
    glBindVertexArray(_vao);
#endif
    glDrawArrays(GL_TRIANGLES, 0, GRID_WIDTH * GRID_HEIGHT * 6);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cout << "Error: " << error << std::endl;
    }
}

void GameView::move() {
    timeval tmpTime = _lastTime;
    gettimeofday(&_lastTime, 0);
    float frame_time = (float)(_lastTime.tv_sec - tmpTime.tv_sec) + (float)(_lastTime.tv_usec - tmpTime.tv_usec) * 0.000001f;
    const float dt = 0.1f;
    
    //std::cout << "FPS: " << 1.0f / frame_time << std::endl;
    
    _time_accumulator += frame_time;
    
    bool needs_redraw = false;
    while (_time_accumulator >= dt) {
        _game.update(dt);
        _time_accumulator -= dt;
        needs_redraw = true;
    }
    
    if (needs_redraw) {
        update_vertex_buffer();
        update();
    }
}

void GameView::showEvent(QShowEvent * /* event */)
{
    _timer_id = startTimer(50);
}

void GameView::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == _timer_id)
    {
        move();
    }
    else {
        QGLWidget::timerEvent(event);
    }
}

void GameView::keyPressEvent(QKeyEvent *event)
{
    handle_keyboard_event(event, KEY_PRESS_EVENT);
}

void GameView::keyReleaseEvent(QKeyEvent *event) {
    handle_keyboard_event(event, KEY_RELEASE_EVENT);
}

void GameView::handle_keyboard_event(QKeyEvent* event, KeyEventType type) {
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
            
        default: QGLWidget::keyPressEvent(event); break;
    }
    
    if (key != KEY_NONE) {
        _game.handle_keyboard_event(KeyEvent(key, type));
    }
}

////////////////////////////////////////////////////////////////////////////////

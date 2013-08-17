
#ifndef GAMEVIEW_H
#define GAMEVIEW_H

// Make Qt load the OpenGL 3 functions
#define GL3_PROTOTYPES

#include <QGLWidget>
#include <vector>
#include <sys/time.h>

#include "Game.h"

class GameView : public QGLWidget
{
    Q_OBJECT
public:
    enum { GRID_WIDTH = Game::GRID_WIDTH };
    enum { GRID_HEIGHT = Game::GRID_HEIGHT };
    enum { TILE_WIDTH = 16 };
    enum { TILE_HEIGHT = 16 };
    enum { VIEW_WIDTH = GRID_WIDTH * TILE_WIDTH };
    enum { VIEW_HEIGHT = GRID_HEIGHT * TILE_HEIGHT };
    
    explicit GameView(const QGLFormat& format, QWidget *parent = 0);

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();

    void showEvent(QShowEvent* event);
    void timerEvent(QTimerEvent* event);
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseDoubleClickEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
private:
    void compile_shader(GLuint shader_identifier, const std::string& shader_source);
    void link_program();
    void update_vertex_buffer();
    void move();
    
    Game _game;

    timeval _lastTime;
    float _time_accumulator;

    int _timer_id;
    GLuint _vao;
    GLuint _vertex_buffer_identifier;
    GLuint _program_identifier;
    GLuint _texture_identifier;
    
};

#endif // GAMEVIEW_H

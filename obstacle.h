#ifndef OBSTACLE_H
#define OBSTACLE_H


#include <GL/gl.h>
#include <GL/glu.h>
#include <cstdio>
#include <math.h>


class Obstacle {
    GLfloat gX;
    GLfloat gY;

    GLfloat gWidth;
    GLfloat gHeight;
    
    GLfloat gRed;
    GLfloat gGreen;
    GLfloat gBlue;


private:
    void DrawObstacle();
    void DrawReact(GLfloat width, GLfloat height, GLfloat R, GLfloat G, GLfloat B);


public:
    Obstacle(GLfloat x, GLfloat y, GLfloat width, GLfloat height) {
        gX = x;
        gY = y;
        gWidth = width;
        gHeight = height;
        gRed = 0.0f;
        gGreen = 0.0f;
        gBlue = 0.0f;
    }

    void Draw() {
        DrawObstacle();
    }

    GLfloat GetGx();
    GLfloat GetGy();
    GLfloat GetWidth();
    GLfloat GetHeight();
};



#endif // OBSTACLE_H
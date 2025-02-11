#ifndef SHOT_H
#define SHOT_H



#include <GL/gl.h>
#include <GL/glu.h>
#include <cstdio>



class Shot {
    GLfloat gXInit; 
    GLfloat gYInit;

    GLfloat gX; 
    GLfloat gY;

    GLfloat gSpeed;
    GLfloat gDirection[2];
    GLfloat gRadius;
    GLfloat gMaxDist;


private:
    void DrawShot(GLfloat x, GLfloat y);
    void DrawCircle(GLfloat radius, GLfloat R, GLfloat G, GLfloat B);


public:
    Shot(GLfloat x, GLfloat y, GLfloat playerSpeed, GLfloat* d, GLfloat maxDist, GLfloat radius) {
        gXInit = x;
        gYInit = y;
        gX = gXInit;
        gY = gYInit;
        gSpeed = 2 * playerSpeed;
        gDirection[0] = d[0];
        gDirection[1] = d[1];
        gRadius = radius;
        gMaxDist = maxDist;
    }

    void Draw() {
        DrawShot(gX, gY);
    }

    void Move(GLdouble timeDifference);
    bool Valid();
    GLfloat GetGx();
    GLfloat GetGy();
    GLfloat GetRadius();
};



#endif // SHOT_H
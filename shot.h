#ifndef SHOT_H
#define SHOT_H



#include <GL/gl.h>
#include <GL/glu.h>
#include <cstdio>



class Shot {
    GLfloat gXInit; 
    GLfloat gYInit;
    GLfloat gZInit;

    GLfloat gX; 
    GLfloat gY;
    GLfloat gZ;

    GLfloat gSpeed;
    GLfloat gDirection[3];
    GLfloat gRadius;
    GLfloat gMaxDist;


private:
    void DrawShot(GLfloat x, GLfloat y, GLfloat z);
    void DrawSphere(GLfloat radius, GLfloat R, GLfloat G, GLfloat B);


public:
    Shot(GLfloat x, GLfloat y, GLfloat z, GLfloat playerSpeed, GLfloat* d, GLfloat maxDist, GLfloat radius) {
        gXInit = x;
        gYInit = y;
        gZInit = z;
        gX = gXInit;
        gY = gYInit;
        gZ = gZInit;
        gSpeed = 2 * playerSpeed;
        gDirection[0] = d[0];
        gDirection[1] = d[1];
        gDirection[2] = d[2];
        gRadius = radius;
        gMaxDist = maxDist;
    }

    void Draw() {
        DrawShot(gX, gY, gZ);
    }

    void Move(GLdouble timeDifference);
    bool Valid();
    GLfloat GetGx();
    GLfloat GetGy();
    GLfloat GetGz();
    GLfloat GetRadius();
};



#endif // SHOT_H
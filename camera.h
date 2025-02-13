#ifndef CAMERA_H
#define CAMERA_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

class Camera {
    GLfloat gX;
    GLfloat gY;
    GLfloat gZ;

    GLfloat gLookAtX;
    GLfloat gLookAtY;
    GLfloat gLookAtZ;

    GLfloat gUpX;
    GLfloat gUpY;
    GLfloat gUpZ;

    GLfloat camXYAngle;
    GLfloat camXZAngle;

    GLfloat latestX;
    GLfloat latestY;

private:


public:
    Camera(GLfloat x, GLfloat y, GLfloat z, GLfloat lookAtX, GLfloat lookAtY, GLfloat lookAtZ, GLfloat upX, GLfloat upY, GLfloat upZ) {
        gX = x;
        gY = y;
        gZ = z;

        gLookAtX = lookAtX;
        gLookAtY = lookAtY;
        gLookAtZ = lookAtZ;

        gUpX = upX;
        gUpY = upY;
        gUpZ = upZ;
    }

    GLfloat GetX();
    GLfloat GetY();
    GLfloat GetZ();
    GLfloat GetLookAtX();
    GLfloat GetLookAtY();
    GLfloat GetLookAtZ();
    GLfloat GetUpX();
    GLfloat GetUpY();
    GLfloat GetUpZ();
};



#endif // CAMERA_H
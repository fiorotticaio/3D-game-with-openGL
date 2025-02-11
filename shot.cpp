#include "shot.h"
#include <math.h>


void Shot::DrawShot(GLfloat x, GLfloat y) {
    glPushMatrix();
        glTranslatef(x, y, 0);
        DrawCircle(gRadius, 1.0f, 1.0f, 1.0f);
    glPopMatrix();
}


void Shot::DrawCircle(GLfloat radius, GLfloat R, GLfloat G, GLfloat B) {
    glColor3f(R, G, B);

    glBegin(GL_POLYGON);
        for (int i = 0; i < 360; i += 20) {
            GLfloat angle = 2.0f * M_PI * i / 360;
            GLfloat x = radius * cos(angle);
            GLfloat y = radius * sin(angle);   
            glVertex2f(x, y);
        }
    glEnd();
}


void Shot::Move(GLdouble timeDifference) {
    gX += gDirection[0] * gSpeed * timeDifference;
    gY += gDirection[1] * gSpeed * timeDifference;
}


bool Shot::Valid() {
    // Checks if the shot has already traveled a maximum distance
    return sqrt(pow(gX - gXInit, 2) + pow(gY - gYInit, 2)) < gMaxDist;
}


GLfloat Shot::GetGx() {
    return gX;
}


GLfloat Shot::GetGy() {
    return gY;
}


GLfloat Shot::GetRadius() {
    return gRadius;
}
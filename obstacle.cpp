#include "obstacle.h"

void Obstacle::DrawObstacle() {
    glPushMatrix();
        glTranslatef(gX, gY, 0);
        DrawReact(gWidth, gHeight, gRed, gGreen, gBlue);
    glPopMatrix();
}

void Obstacle::DrawReact(GLfloat width, GLfloat height, GLfloat R, GLfloat G, GLfloat B) {
    glColor3f(R, G, B);

    // The coordenates givem in the svg file are the left top corner of the rectangle
    glBegin(GL_POLYGON);
        glVertex2f(0, -height);
        glVertex2f(width, -height);
        glVertex2f(width, 0);
        glVertex2f(0, 0);
    glEnd();
}


GLfloat Obstacle::GetGx() {
    return gX;
}


GLfloat Obstacle::GetGy() {
    return gY;
}


GLfloat Obstacle::GetWidth() {
    return gWidth;
}


GLfloat Obstacle::GetHeight() {
    return gHeight;
}
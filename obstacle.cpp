#include "obstacle.h"

void Obstacle::DrawObstacle() {
    GLfloat materialEmission[] = { 0.00, 0.00, 0.00, 1.0};
    GLfloat materialColor[] = { 0.0, 0.0, 0.0, 1.0};
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0};
    GLfloat mat_shininess[] = { 128 };
    
    glPushMatrix();
        glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
        glMaterialfv(GL_FRONT, GL_AMBIENT, materialColor);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColor);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
        glColor3f(0,0,1);

        glTranslatef(gX, gY, 1);
        DrawReact(gWidth, gHeight, gRed, gGreen, gBlue);
    glPopMatrix();
}

void Obstacle::DrawReact(GLfloat width, GLfloat height, GLfloat R, GLfloat G, GLfloat B) {
    glColor3f(R, G, B);

    // The coordenates givem in the svg file are the left top corner of the rectangle
    glBegin(GL_POLYGON);
        glVertex3f(0, -height, 1);
        glNormal3f(0, 0, 1);
        glVertex3f(width, -height, 1);
        glNormal3f(0, 0, 1);
        glVertex3f(width, 0, 1);
        glNormal3f(0, 0, 1);
        glVertex3f(0, 0, 1);
        glNormal3f(0, 0, 1);
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
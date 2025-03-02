#include "obstacle.h"

void Obstacle::DrawObstacle() {
    GLfloat materialEmission[] = { 0.00, 0.00, 0.00, 1.0};
    GLfloat materialColor[] = { 0.0, 0.0, 0.0, 1.0};
    GLfloat mat_specular[] = { 0.5, 0.5, 0.5, 1.0 };
    GLfloat mat_shininess[] = { 50 };
    
    glPushMatrix();
        glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
        glMaterialfv(GL_FRONT, GL_AMBIENT, materialColor);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColor);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
        glColor3f(0, 0, 1);

        glTranslatef(gX, gY, 1);
        DrawReacts(gWidth, gHeight, gThickness, gRed, gGreen, gBlue);
    glPopMatrix();
}

void Obstacle::DrawReacts(GLfloat width, GLfloat height, GLfloat thickness, GLfloat R, GLfloat G, GLfloat B) {
    glColor3f(R, G, B);

    // The coordenates givem in the svg file are the left top corner of the rectangle

    // 1º Face
    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);
        glVertex3f(0, -height, 0);
        glNormal3f(0, 0, 1);
        glVertex3f(width, -height, 0);
        glNormal3f(0, 0, 1);
        glVertex3f(width, 0, 0);
        glNormal3f(0, 0, 1);
        glVertex3f(width, 0, 0);
    glEnd();

    // 2º Face
    glBegin(GL_QUADS);
        glNormal3f(1, 0, 0);
        glVertex3f(width, -height, 0);
        glNormal3f(1, 0, 0);
        glVertex3f(width, -height, -thickness);
        glNormal3f(1, 0, 0);
        glVertex3f(width, 0, -thickness);
        glNormal3f(1, 0, 0);
        glVertex3f(width, 0, 0);
    glEnd();

    // 3º Face
    glBegin(GL_QUADS);
        glNormal3f(0, 0, -1);
        glVertex3f(width, -height, -thickness);
        glNormal3f(0, 0, -1);
        glVertex3f(0, -height, -thickness);
        glNormal3f(0, 0, -1);
        glVertex3f(0, 0, -thickness);
        glNormal3f(0, 0, -1);
        glVertex3f(width, 0, -thickness);
    glEnd();

    // 4º Face
    glBegin(GL_QUADS);
        glNormal3f(-1, 0, 0);
        glVertex3f(0, -height, -thickness);
        glNormal3f(-1, 0, 0);
        glVertex3f(0, -height, 0);
        glNormal3f(-1, 0, 0);
        glVertex3f(0, 0, 0);
        glNormal3f(-1, 0, 0);
        glVertex3f(0, 0, -thickness);
    glEnd();

    // Ground
    glBegin(GL_QUADS);
        glNormal3f(0, -1, 0);
        glVertex3f(0, -height, 0);
        glNormal3f(0, -1, 0);
        glVertex3f(0, -height, -thickness);
        glNormal3f(0, -1, 0);
        glVertex3f(width, -height, -thickness);
        glNormal3f(0, -1, 0);
        glVertex3f(width, -height, 0);
    glEnd();

    // Ceiling
    glBegin(GL_QUADS);
        glNormal3f(0, 1, 0);
        glVertex3f(0, 0, 0);
        glNormal3f(0, 1, 0);
        glVertex3f(width, 0, 0);
        glNormal3f(0, 1, 0);
        glVertex3f(width, 0, -thickness);
        glNormal3f(0, 1, 0);
        glVertex3f(0, 0, -thickness);
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
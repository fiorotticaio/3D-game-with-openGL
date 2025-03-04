#include "shot.h"
#include <math.h>


void Shot::DrawShot(GLfloat x, GLfloat y, GLfloat z) {
    glPushMatrix();
        glTranslatef(x, y, z);
        DrawSphere(gRadius, 1.0f, 1.0f, 1.0f);
    glPopMatrix();
}


void Shot::DrawSphere(GLfloat radius, GLfloat R, GLfloat G, GLfloat B) {
    GLfloat materialEmission[] = { 0.00, 0.00, 0.00, 1.0 };
    GLfloat materialColor[] = { R, G, B, 1.0 };
    GLfloat mat_specular[] = { 0.5, 0.5, 0.5, 1.0 };
    GLfloat mat_shininess[] = { 50 };

    glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialColor);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColor);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glColor3f(R, G, B);

    GLUquadric* quad = gluNewQuadric();
    gluSphere(quad, radius, 20, 20);
    gluDeleteQuadric(quad);
}


void Shot::Move(GLdouble timeDifference) {
    gX += gDirection[0] * gSpeed * timeDifference;
    gY += gDirection[1] * gSpeed * timeDifference;
    gZ += gDirection[2] * gSpeed * timeDifference;
}


bool Shot::Valid() {
    // Verifica se o tiro já percorreu a distância máxima
    return sqrt(pow(gX - gXInit, 2) + pow(gY - gYInit, 2) + pow(gZ - gZInit, 2)) < gMaxDist;
}


GLfloat Shot::GetGx() {
    return gX;
}


GLfloat Shot::GetGy() {
    return gY;
}


GLfloat Shot::GetGz() {
    return gZ;
}


GLfloat Shot::GetRadius() {
    return gRadius;
}
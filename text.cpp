#include "text.h"

void RasterChars(GLfloat x, GLfloat y, GLfloat z, const char* text, double r, double g, double b) {
    glPushAttrib(GL_ENABLE_BIT);
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);

        glColor3f(r, g, b);
        glRasterPos3f(x, y, z);
        const char* tmpStr;
        tmpStr = text;

        while (*tmpStr) {
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *tmpStr);
            tmpStr++;
        }
        glEnable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);
    glPopAttrib();
}

void PrintTextUI(GLfloat x, GLfloat y, const char* text, double r, double g, double b) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
        glLoadIdentity();
        glOrtho(0, 1, 0, 1, -1, 1);
        RasterChars(x, y, 0, text, r, g, b);    
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void PrintTextWorld(GLfloat x, GLfloat y, GLfloat z, const char* text, double r, double g, double b) {
    glPushAttrib(GL_ENABLE_BIT);
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        glColor3f(r, g, b);

        // Set raster position in world coordinates
        glRasterPos3f(x, y, z);

        // Render each character
        const char* tmpStr = text;
        while (*tmpStr) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, *tmpStr);
            tmpStr++;
        }

        glEnable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);
    glPopAttrib();
}

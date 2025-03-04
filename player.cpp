#include "player.h"


Player::Player(GLfloat x, GLfloat y, GLfloat baseCircleRadius, GLfloat arenaThickness)
    : Character(x, y, baseCircleRadius, arenaThickness) {
}


void Player::Draw() {
    glPushMatrix();
        // Draw the body
        glTranslatef(gX, gY, gZ);
        glRotatef(gXZAngle, 0, 1, 0);
        DrawCuboid(gBodyWidth, gBodyHeight, gBodyThickness, 0.0f, 1.0f, 0.0f);

        DrawHeadAndArms(0.0f, 1.0f, 0.0f);

        DrawBackLeg(1.0f, 0.0f, 0.0f);
        
        DrawFrontLeg(1.0f, 0.0f, 0.0f);
    glPopMatrix();
}
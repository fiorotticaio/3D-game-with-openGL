#include "player.h"


Player::Player(GLfloat x, GLfloat y, GLfloat baseCircleRadius, GLfloat arenaThickness)
    : Character(x, y, baseCircleRadius, arenaThickness) {
        gIsPlayer = true;

        gTShirtFrontTexture = LoadTextureRAW("textures/t-shirt-front.bmp");
        gTShirtBaseTexture = LoadTextureRAW("textures/t-shirt-base.bmp");
}


void Player::Draw() {
    glPushMatrix();
        // Draw the body
        glTranslatef(gX, gY, gZ);
        glRotatef(gXZAngle, 0, 1, 0);
        DrawCuboid(gBodyWidth, gBodyHeight, gBodyThickness, 0.0f, 0.0f, 1.0f, gTShirtFrontTexture);

        DrawHeadAndArms(0.0f, 0.0f, 1.0f);

        DrawBackLeg(0.0f, 0.0f, 1.0f);
        
        DrawFrontLeg(0.0f, 0.0f, 1.0f);
    glPopMatrix();
}
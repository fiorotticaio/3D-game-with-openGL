#include "opponent.h"


Opponent::Opponent(GLfloat x, GLfloat y, GLfloat baseCircleRadius, GLfloat arenaThickness)
    : Character(x, y, baseCircleRadius, arenaThickness) {
    gCollideWithRightX = true;
    gCollideWithLeftX = true;
    gCollideWithRightZ = true;
    gCollideWithLeftZ = true;
}


void Opponent::Draw() {
    glPushMatrix();
        // Draw the body
        glTranslatef(gX, gY, gZ);
        glRotatef(gXZAngle, 0, 1, 0);
        DrawCuboid(gBodyWidth, gBodyHeight, gBodyThickness, 1.0f, 0.0f, 0.0f);

        DrawHeadAndArms(1.0f, 0.0f, 0.0f);

        DrawBackLeg(1.0f, 0.0f, 0.0f);
        
        DrawFrontLeg(1.0f, 0.0f, 0.0f);
    glPopMatrix();
}


void Opponent::RotateArmToTargetAngle(GLdouble timeDifference, GLfloat XZtargetAngle , GLfloat XYtargetAngle) {
    if (gXYArmAngle < XYtargetAngle) {
        gXYArmAngle += gArmSpeed * timeDifference;
    } else if (gXYArmAngle > XYtargetAngle) {
        gXYArmAngle -= gArmSpeed * timeDifference;
    }

    if (gXZArmAngle < XZtargetAngle) {
        gXZArmAngle += gArmSpeed * timeDifference;
    } else if (gXZArmAngle > XZtargetAngle) {
        gXZArmAngle -= gArmSpeed * timeDifference;
    }
}


bool Opponent::GetCollideWithRightX() {
    return gCollideWithRightX;
}


bool Opponent::GetCollideWithLeftX() {
    return gCollideWithLeftX;
}


bool Opponent::GetCollideWithRightZ() {
    return gCollideWithRightZ;
}


bool Opponent::GetCollideWithLeftZ() {
    return gCollideWithLeftZ;
}


void Opponent::SetCollideWithRightX(bool collideWithRightX) {
    gCollideWithRightX = collideWithRightX;
}


void Opponent::SetCollideWithLeftX(bool collideWithLeftX) {
    gCollideWithLeftX = collideWithLeftX;
}


void Opponent::SetCollideWithRightZ(bool collideWithRightZ) {
    gCollideWithRightZ = collideWithRightZ;
}


void Opponent::SetCollideWithLeftZ(bool collideWithLeftZ) {
    gCollideWithLeftZ = collideWithLeftZ;
}
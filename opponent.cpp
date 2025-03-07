#include "opponent.h"

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
#include "opponent.h"

void Opponent::DrawOpponent() {
    glPushMatrix();
        // Draw the body
        glTranslatef(gX, gY, 0);
        DrawRect(gBodyWidth, gBodyHeight, 0.0f, 1.0f, 1.0f);

        DrawHeadAndArm();

        DrawBackLeg();

        DrawFrontLeg();
    glPopMatrix();
}

void Opponent::DrawHeadAndArm() {
    glPushMatrix();
        // Draw the head
        glTranslatef(0, gBodyHeight + gHeadCircleRadius, 0);
        DrawCircle(gHeadCircleRadius, 1.0f, 0.0f, 0.0f);

        // Draw the arm
        glTranslatef(0, -(gHeadCircleRadius + (gBodyHeight/2)), 0);
        glRotatef(gArmAngle * gXDirection, 0, 0, 1);
        DrawRect(gArmWidth, gArmHeight, 1.0f, 1.0f, 0.0f);
    glPopMatrix();
}

void Opponent::DrawFrontLeg() {
    glPushMatrix();
        // Draw the front thigh
        glRotatef(gFrontThighAngle * gXDirection, 0, 0, 1);
        DrawRect(gThighWidth, gThighHeight, 1.0f, 0.0f, 0.0f);

        // Draw the front leg
        glTranslatef(0, gThighHeight, 0);
        glRotatef(gFrontShinAngle * gXDirection, 0, 0, 1);
        DrawRect(gShinWidth, gShinHeight, 1.0f, 0.0f, 0.0f);
    glPopMatrix();
}

void Opponent::DrawBackLeg() {
    glPushMatrix();
        // Draw the back thigh
        glRotatef(gBackThighAngle * gXDirection, 0, 0, 1);
        DrawRect(gThighWidth, gThighHeight, 1.0f, 0.0f, 0.0f);

        // Draw the back leg
        glTranslatef(0, gThighHeight, 0);
        glRotatef(gBackShinAngle * gXDirection, 0, 0, 1);
        DrawRect(gShinWidth, gShinHeight, 1.0f, 0.0f, 0.0f);
    glPopMatrix();
}

void Opponent::DrawRect(GLfloat width, GLfloat height, GLfloat R, GLfloat G, GLfloat B) {
    glColor3f(R, G, B);

    glBegin(GL_POLYGON);
        glVertex2f(-width / 2, 0);
        glVertex2f(width / 2, 0);
        glVertex2f(width / 2, height);
        glVertex2f(-width / 2, height);
    glEnd();
}

void Opponent::DrawCircle(GLfloat radius, GLfloat R, GLfloat G, GLfloat B) {
    const int numSegments = 100;
    const GLfloat angleStep = 2.0f * M_PI / numSegments;

    glColor3f(R, G, B);

    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(0.0f, 0.0f);
        for (int i = 0; i <= numSegments; ++i) {
            GLfloat angle = i * angleStep;
            GLfloat x = radius * cos(angle);
            GLfloat y = radius * sin(angle);
            glVertex2f(x, y);
        }
    glEnd();
}

GLfloat Opponent::GetGx() {
    return gX;
}

GLfloat Opponent::GetGy() {
    return gY;
}

void Opponent::MoveInX(GLfloat minOpponentPositionX, GLfloat maxOpponentPositionX, GLdouble timeDifference) {
    if (gX + gXSpeed * timeDifference * gXDirection >= minOpponentPositionX + gBodyWidth/2 && 
        gX + gXSpeed * timeDifference * gXDirection <= maxOpponentPositionX - gBodyWidth/2) {
        AnimateLegs(timeDifference);
        gX += gXSpeed * timeDifference * gXDirection;
    }
}

void Opponent::MoveInY(GLfloat minOpponentPositionY, GLfloat maxOpponentPositionY, GLdouble timeDifference) {
    if (gY + gYSpeed * timeDifference * gYDirection >= minOpponentPositionY + gThighHeight + gShinHeight && 
        gY + gYSpeed * timeDifference * gYDirection <= maxOpponentPositionY - gThighHeight - gShinHeight) {
        gY += gYSpeed * timeDifference * gYDirection;
        jumpHeight += gYSpeed * timeDifference * gYDirection;
    }
}

void Opponent::RotateArmToTargetAngle(GLdouble timeDifference, GLfloat targetAngle) {
    if (gArmAngle < targetAngle) {
        gArmAngle += gArmSpeed * timeDifference;
    } else if (gArmAngle > targetAngle) {
        gArmAngle -= gArmSpeed * timeDifference;
    }
}

void Opponent::SetXDirection(GLint xDirection) {
    gXDirection = xDirection;
}

void Opponent::SetYDirection(GLint yDirection) {
    gYDirection = yDirection;
}

GLfloat Opponent::GetFrontThighAngle() {
    return gFrontThighAngle;
}

GLfloat Opponent::GetBackThighAngle() {
    return gBackThighAngle;
}

GLfloat Opponent::GetFrontShinAngle() {
    return gFrontShinAngle;
}

GLfloat Opponent::GetBackShinAngle() {
    return gBackShinAngle;
}

void Opponent::RotateFrontThigh(GLfloat angle, GLdouble timeDifference) {
    gFrontThighAngle += angle * 0.1f * timeDifference;
}

void Opponent::RotateBackThigh(GLfloat angle, GLdouble timeDifference) {
    gBackThighAngle += angle * 0.1f * timeDifference;
}

void Opponent::RotateFrontShin(GLfloat angle, GLdouble timeDifference) {
    gFrontShinAngle += angle * 0.1f * timeDifference;
}

void Opponent::RotateBackShin(GLfloat angle, GLdouble timeDifference) {
    gBackShinAngle += angle * 0.1f * timeDifference;
}

void Opponent::SetFrontShinAngle(GLfloat angle) {
    gFrontShinAngle = angle;
}

void Opponent::SetBackShinAngle(GLfloat angle) {
    gBackShinAngle = angle;
}

// Aux rotation func
void Opponent::RotatePoint(GLfloat x, GLfloat y, GLfloat angle, GLfloat &xOut, GLfloat &yOut) {
    // Rotation matrix
    GLfloat rotMatrix[3][3] = {
        {cos(angle), -sin(angle), 0},
        {sin(angle), cos(angle) , 0},
        {0         , 0          , 1},
    };

    // Input vector
    GLfloat inVector[3] = {x, y, 1};

    // Output vector
    GLfloat outVector[3] = {0, 0, 0};

    // Multiply matrix by vector
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            outVector[i] += rotMatrix[i][j] * inVector[j];
        }
    }

    // Output values
    xOut = outVector[0];
    yOut = outVector[1];
}

// Aux translate func
void Opponent::TranslatePoint(GLfloat x, GLfloat y, GLfloat dx, GLfloat dy, GLfloat &xOut, GLfloat &yOut) {
    // Translation matrix
    GLfloat transMatrix[3][3] = {
        {1, 0, dx},
        {0, 1, dy},
        {0, 0, 1},
    };

    // Input vector
    GLfloat inVector[3] = {x, y, 1};

    // Output vector
    GLfloat outVector[3] = {0, 0, 0};

    // Multiply matrix by vector
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            outVector[i] += transMatrix[i][j] * inVector[j];
        }
    }

    // Output values
    xOut = outVector[0];
    yOut = outVector[1];
}

// Aux scale func
void Opponent::ScalePoint(GLfloat x, GLfloat y, GLfloat sx, GLfloat sy, GLfloat &xOut, GLfloat &yOut) {
    // Sacle matriz
    GLfloat scaleMatrix[3][3] = {
        {sx, 0 , 0},
        {0 , sy, 0},
        {0 , 0 , 1},
    };

    // Input vector
    GLfloat inVector[3] = {x, y, 1};

    // Output vector
    GLfloat outVector[3] = {0, 0, 0};

    // Multiply matrix by vector
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            outVector[i] += scaleMatrix[i][j] * inVector[j];
        }
    }

    // Output values
    xOut = outVector[0];
    yOut = outVector[1];
}

Shot* Opponent::Shoot(GLfloat maxDist) {
    GLfloat xBaseArm = 0.0, yBaseArm = 0.0;
    GLfloat xTopArm = 0.0, yTopArm = 0.0;

    glPushMatrix();
        // Getting top position
        GLfloat x = 0.0, y = 0.0;
        GLfloat xOut = 0.0, yOut = 0.0;

        TranslatePoint(x, y, 0, gArmHeight, xOut, yOut);
        x = xOut; y = yOut;

        RotatePoint(x, y, (gArmAngle * M_PI / 180) * gXDirection, xOut, yOut);
        x = xOut; y = yOut;

        TranslatePoint(x, y, 0, gBodyHeight/2, xOut, yOut);
        x = xOut; y = yOut;

        TranslatePoint(x, y, gX, gY, xOut, yOut);
        x = xOut; y = yOut;

        xTopArm = x;
        yTopArm = y;


        // Getting base position
        x = 0.0; y = 0.0;
        xOut = 0.0, yOut = 0.0;

        RotatePoint(x, y, (gArmAngle * M_PI / 180) * gXDirection, xOut, yOut);
        x = xOut; y = yOut;

        TranslatePoint(x, y, 0, gBodyHeight/2, xOut, yOut);
        x = xOut; y = yOut;

        TranslatePoint(x, y, gX, gY, xOut, yOut);
        x = xOut; y = yOut;

        xBaseArm = x;
        yBaseArm = y;


        // Findig the direction
        GLfloat baseVector[2] = {xBaseArm, yBaseArm};
        GLfloat topVector[2] = {xTopArm, yTopArm};

        GLfloat xResVector = topVector[0] - baseVector[0];
        GLfloat yResVector = topVector[1] - baseVector[1];

        GLfloat norm = sqrt(pow(xResVector, 2) + pow(yResVector, 2));
        xResVector /= norm;
        yResVector /= norm;

        GLfloat shotDirection[2] = {xResVector, yResVector};
    glPopMatrix();

    return new Shot(xTopArm, yTopArm, gXSpeed, shotDirection, maxDist, gBaseCircleRadius/5);
}


GLfloat Opponent::GetXSpeed() {
    return gXSpeed;
}


GLfloat Opponent::GetYSpeed() {
    return gYSpeed;
}


GLint Opponent::GetXDirection() {
    return gXDirection;
}


GLint Opponent::GetYDirection() {
    return gYDirection;
}


void Opponent::Jump() {
    gYDirection = 1;
    jumpHeight = 0; // Reset jump height
}


GLfloat Opponent::GetMaxJumpHeight() {
    return maxJumpHeight;
}


GLfloat Opponent::GetJumpHeight() {
    return jumpHeight;
}


GLfloat Opponent::GetThighHeight() {
    return gThighHeight;
}


GLfloat Opponent::GetShinHeight() {
    return gShinHeight;
}


bool Opponent::ReachedMaximumJumpHeight() {
    return jumpHeight >= maxJumpHeight;
}


GLfloat Opponent::GetInvisibleReactHeight() {
    return gInvisibleReactHeight;
}


GLfloat Opponent::GetInvisibleReactWidth() {
    return gInvisibleReactWidth;
}


void Opponent::AnimateLegs(GLdouble timeDifference) {
    if (gFrontThighAngle > -140) gFrontThighAngleDir *= -1;
    if (gFrontThighAngle < -210) gFrontThighAngleDir *= -1;
    if (gBackThighAngle > -140) gBackThighAngleDir *= -1;
    if (gBackThighAngle < -210) gBackThighAngleDir *= -1;

    if (gFrontShinAngle > 0) gFrontShinAngleDir *= -1;
    if (gFrontShinAngle < -50) gFrontShinAngleDir *= -1;
    if (gBackShinAngle > 0) gBackShinAngleDir *= -1;
    if (gBackShinAngle < -50) gBackShinAngleDir *= -1;

    RotateFrontThigh(gFrontThighAngleDir, timeDifference);
    RotateBackThigh(gBackThighAngleDir, timeDifference);
    RotateFrontShin(gFrontShinAngleDir, timeDifference);
    RotateBackShin(gBackShinAngleDir, timeDifference);
}
#include "opponent.h"

void Opponent::DrawOpponent() {
    glPushMatrix();
        // Draw the body
        glTranslatef(gX, gY, gZ);
        glRotatef(gXZAngle, 0, 1, 0);
        DrawCuboid(gBodyWidth, gBodyHeight, gBodyThickness, 1.0f, 0.0f, 0.0f);

        DrawHeadAndArms();

        DrawBackLeg();
        
        DrawFrontLeg();
    glPopMatrix();
}


void Opponent::DrawHeadAndArms() {
    glPushMatrix();
        // Draw the head
        glTranslatef(0, gBodyHeight + gHeadCircleRadius, 0);
        DrawSphere(gHeadCircleRadius, 1.0f, 0.0f, 0.0f);

        // Draw front arm (the one that moves)
        glTranslatef(0, -(gHeadCircleRadius + (gBodyHeight/2)), gBodyThickness/2);

        glPushMatrix(); // The angle dont interfear in the angle of the other arm
            glRotatef(gArmAngle, 0, 0, 1);
            DrawCuboid(gArmWidth, gArmHeight, gArmThickness, 1.0f, 1.0f, 0.0f);
        glPopMatrix();

        // Draw back arm (the one that doesn't move)
        glTranslatef(0, 0, -gBodyThickness);
        glRotatef(-150, 0, 0, 1); // Fixed angle
        DrawCuboid(gArmWidth, gArmHeight, gArmThickness, 1.0f, 1.0f, 0.0f);
    glPopMatrix();
}


void Opponent::DrawSphere(GLfloat radius, GLfloat R, GLfloat G, GLfloat B) {
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


void Opponent::DrawFrontLeg() {
    glPushMatrix();
        // Draw the front thigh
        glTranslatef(0, 0, -gBodyThickness/2);
        glRotatef(gFrontThighAngle, 0, 0, 1);
        DrawCuboid(gThighWidth, gThighHeight, gThighThickness, 1.0f, 1.0f, 1.0f);

        // Draw the front leg
        glTranslatef(0, gThighHeight, 0);
        glRotatef(gFrontShinAngle, 0, 0, 1);
        DrawCuboid(gShinWidth, gShinHeight, gShinThickness, 1.0f, 1.0f, 1.0f);
    glPopMatrix();
}


void Opponent::DrawBackLeg() {
    glPushMatrix();
        // Draw the back thigh
        glTranslatef(0, 0, gBodyThickness/2);
        glRotatef(gBackThighAngle, 0, 0, 1);
        DrawCuboid(gThighWidth, gThighHeight, gThighThickness, 1.0f, 1.0f, 1.0f);

        // Draw the back shin
        glTranslatef(0, gThighHeight, 0);
        glRotatef(gBackShinAngle, 0, 0, 1);
        DrawCuboid(gShinWidth, gShinHeight, gShinThickness, 1.0f, 1.0f, 1.0f);
    glPopMatrix();
}


void Opponent::DrawCuboid(GLfloat width, GLfloat height, GLfloat depth, GLfloat R, GLfloat G, GLfloat B) {
    GLfloat materialEmission[] = { 0.00, 0.00, 0.00, 1.0 };
    GLfloat materialColor[] = { R, G, B, 1.0 };
    GLfloat mat_specular[] = { 0.5, 0.5, 0.5, 1.0 };
    GLfloat mat_shininess[] = { 50 };

    
    glBegin(GL_QUADS);
        glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
        glMaterialfv(GL_FRONT, GL_AMBIENT, materialColor);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColor);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
        glColor3f(R, G, B);

        // Front face
        glNormal3f(0, 0, 1);
        glVertex3f(-width/2, 0, depth/2);
        glNormal3f(0, 0, 1);
        glVertex3f(width/2, 0, depth/2);
        glNormal3f(0, 0, 1);
        glVertex3f(width/2, height, depth/2);
        glNormal3f(0, 0, 1);
        glVertex3f(-width/2, height, depth/2);

        // Back face
        glNormal3f(0, 0, -1);
        glVertex3f(-width/2, 0, -depth/2);
        glNormal3f(0, 0, -1);
        glVertex3f(-width/2, height, -depth/2);
        glNormal3f(0, 0, -1);
        glVertex3f(width/2, height, -depth/2);
        glNormal3f(0, 0, -1);
        glVertex3f(width/2, 0, -depth/2);

        // Right face
        glNormal3f(1, 0, 0);
        glVertex3f(width/2, 0, depth/2);
        glNormal3f(1, 0, 0);
        glVertex3f(width/2, 0, -depth/2);
        glNormal3f(1, 0, 0);
        glVertex3f(width/2, height, -depth/2);
        glNormal3f(1, 0, 0);
        glVertex3f(width/2, height, depth/2);

        // Left face
        glNormal3f(-1, 0, 0);
        glVertex3f(-width/2, 0, depth/2);
        glNormal3f(-1, 0, 0);
        glVertex3f(-width/2, height, depth/2);
        glNormal3f(-1, 0, 0);
        glVertex3f(-width/2, height, -depth/2);
        glNormal3f(-1, 0, 0);
        glVertex3f(-width/2, 0, -depth/2);

        // Top face
        glNormal3f(0, 1, 0);
        glVertex3f(-width/2, height, depth/2);
        glNormal3f(0, 1, 0);
        glVertex3f(width/2, height, depth/2);
        glNormal3f(0, 1, 0);
        glVertex3f(width/2, height, -depth/2);
        glNormal3f(0, 1, 0);
        glVertex3f(-width/2, height, -depth/2);

        // Bottom face
        glNormal3f(0, -1, 0);
        glVertex3f(-width/2, 0, depth/2);
        glNormal3f(0, -1, 0);
        glVertex3f(-width/2, 0, -depth/2);
        glNormal3f(0, -1, 0);
        glVertex3f(width/2, 0, -depth/2);
        glNormal3f(0, -1, 0);
        glVertex3f(width/2, 0, depth/2);
    glEnd();
}


GLfloat Opponent::GetGx() {
    return gX;
}

GLfloat Opponent::GetGy() {
    return gY;
}

GLfloat Opponent::GetGz() {
    return gZ;
}

void Opponent::MoveInXZ(GLfloat minPlayerPositionX, GLfloat maxPlayerPositionX, GLfloat minPlayerPositionZ, GLfloat maxPlayerPositionZ, GLdouble timeDifference) {

    GLfloat angleRad = gXZAngle * M_PI / 180.0f;
    GLfloat gX_preview = gX + gXZSpeed * timeDifference * gMovementDirection * cos(angleRad);
    GLfloat gZ_preview = gZ - (gXZSpeed * timeDifference * gMovementDirection * sin(angleRad));

    bool playerViolatesXMIN = gX_preview <= minPlayerPositionX + gBodyWidth/2;
    bool playerViolatesXMAX = gX_preview >= maxPlayerPositionX - gBodyWidth/2;
    bool playerViolatesZMIN = gZ_preview <= minPlayerPositionZ + gBodyWidth/2;
    bool playerViolatesZMAX = gZ_preview >= maxPlayerPositionZ - gBodyWidth/2;

    if (!playerViolatesXMIN && !playerViolatesXMAX && !playerViolatesZMIN && !playerViolatesZMAX) {
        AnimateLegs(timeDifference);
        gX = gX_preview;
        gZ = gZ_preview;
    }
}

void Opponent::MoveInY(GLfloat minOpponentPositionY, GLfloat maxOpponentPositionY, GLdouble timeDifference) {
    if (gY + gYSpeed * timeDifference * gYDirection >= minOpponentPositionY + gThighHeight + gShinHeight && 
        gY + gYSpeed * timeDifference * gYDirection <= maxOpponentPositionY - gThighHeight - gShinHeight) {
        gY += gYSpeed * timeDifference * gYDirection;
        jumpHeight += gYSpeed * timeDifference * gYDirection;
    }
}

void Opponent::Rotate(bool clockwise, GLdouble timeDifference) {
    gXZAngle += gArmSpeed * (clockwise ? 1 : -1) * timeDifference; // using arm speed for testing 
    gXZAngle = gXZAngle % 360;
}

void Opponent::RotateArmToTargetAngle(GLdouble timeDifference, GLfloat targetAngle) {
    if (gArmAngle < targetAngle) {
        gArmAngle += gArmSpeed * timeDifference;
    } else if (gArmAngle > targetAngle) {
        gArmAngle -= gArmSpeed * timeDifference;
    }
}

void Opponent::SetXDirection(GLint xDirection) {
    gMovementDirection = xDirection;
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

        RotatePoint(x, y, (gArmAngle * M_PI / 180) * gMovementDirection, xOut, yOut);
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

        RotatePoint(x, y, (gArmAngle * M_PI / 180) * gMovementDirection, xOut, yOut);
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

    return new Shot(xTopArm, yTopArm, gXZSpeed, shotDirection, maxDist, gBaseCircleRadius/5);
}


GLfloat Opponent::GetXSpeed() {
    return gXZSpeed;
}


GLfloat Opponent::GetYSpeed() {
    return gYSpeed;
}


GLint Opponent::GetXDirection() {
    return gMovementDirection;
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
#include "player.h"

void Player::DrawPlayer() {
    glPushMatrix();
        // Draw the body
        glTranslatef(gX, gY, gZ);
        glRotatef(gXZAngle, 0, 1, 0);
        DrawCuboid(gBodyWidth, gBodyHeight, gBodyThickness, 0.0f, 1.0f, 0.0f);

        DrawHeadAndArms();

        DrawBackLeg();
        
        DrawFrontLeg();
    glPopMatrix();
}

void Player::DrawHeadAndArms() {
    glPushMatrix();
        // Draw the head
        glTranslatef(0, gBodyHeight + gHeadCircleRadius, 0);
        DrawSphere(gHeadCircleRadius, 0.0f, 1.0f, 0.0f);

        // Draw front arm (the one that moves)
        glTranslatef(0, -(gHeadCircleRadius + (gBodyHeight/2)), gBodyThickness/2);

        glPushMatrix(); // The angle dont interfear in the angle of the other arm
            glRotatef(gXYArmAngle, 0, 0, 1);
            glRotatef(gXZArmAngle, 1, 0, 0);
            DrawCuboid(gArmWidth, gArmHeight, gArmThickness, 1.0f, 1.0f, 0.0f);
        glPopMatrix();

        // Draw back arm (the one that doesn't move)
        glTranslatef(0, 0, -gBodyThickness);
        glRotatef(-150, 0, 0, 1); // Fixed angle
        DrawCuboid(gArmWidth, gArmHeight, gArmThickness, 1.0f, 1.0f, 0.0f);
    glPopMatrix();
}


void Player::DrawSphere(GLfloat radius, GLfloat R, GLfloat G, GLfloat B) {
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


void Player::DrawFrontLeg() {
    glPushMatrix();
        // Draw the front thigh
        glTranslatef(0, 0, -gBodyThickness/2);
        glRotatef(gFrontThighAngle, 0, 0, 1);
        DrawCuboid(gThighWidth, gThighHeight, gThighThickness, 1.0f, 0.0f, 0.0f);

        // Draw the front leg
        glTranslatef(0, gThighHeight, 0);
        glRotatef(gFrontShinAngle, 0, 0, 1);
        DrawCuboid(gShinWidth, gShinHeight, gShinThickness, 1.0f, 0.0f, 0.0f);
    glPopMatrix();
}


void Player::DrawBackLeg() {
    glPushMatrix();
        // Draw the back thigh
        glTranslatef(0, 0, gBodyThickness/2);
        glRotatef(gBackThighAngle, 0, 0, 1);
        DrawCuboid(gThighWidth, gThighHeight, gThighThickness, 1.0f, 0.0f, 0.0f);

        // Draw the back shin
        glTranslatef(0, gThighHeight, 0);
        glRotatef(gBackShinAngle, 0, 0, 1);
        DrawCuboid(gShinWidth, gShinHeight, gShinThickness, 1.0f, 0.0f, 0.0f);
    glPopMatrix();
}


void Player::DrawCuboid(GLfloat width, GLfloat height, GLfloat depth, GLfloat R, GLfloat G, GLfloat B) {
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


GLfloat Player::GetGx() {
    return gX;
}


GLfloat Player::GetGy() {
    return gY;
}


GLfloat Player::GetGz() {
    return gZ;
}

void Player::MoveInXZ(GLfloat minPlayerPositionX, GLfloat maxPlayerPositionX, GLfloat minPlayerPositionZ, GLfloat maxPlayerPositionZ, GLdouble timeDifference) {

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


void Player::MoveInY(GLfloat minPlayerPositionY, GLfloat maxPlayerPositionY, GLdouble timeDifference) {
    if (gY + gYSpeed * timeDifference * gYDirection >= minPlayerPositionY + gThighHeight + gShinHeight && 
        gY + gYSpeed * timeDifference * gYDirection <= maxPlayerPositionY - gThighHeight - gShinHeight) {
        gY += gYSpeed * timeDifference * gYDirection;
        jumpHeight += gYSpeed * timeDifference * gYDirection;
    }
}

void Player::Rotate(bool clockwise, GLdouble timeDifference) {
    gXZAngle += gRotationSpeed * (clockwise ? 1 : -1) * timeDifference; 
    gXZAngle = gXZAngle % 360;
}

void Player::RotateArm(GLfloat x, GLfloat y, GLfloat windowWidth, GLfloat windowHeight, GLdouble timeDifference) {
    GLfloat mouseXMin = 0;
    GLfloat mouseXMax = windowWidth - mouseXMin;
    GLfloat mouseYMin = 0;
    GLfloat mouseYMax = windowHeight - mouseYMin;

    GLfloat XYMaxAngle = -45.0f;
    GLfloat XYMinAngle = -135.0f;
    GLfloat XYTargetAngle = XYMinAngle + ((y - mouseYMin) / (mouseYMax - mouseYMin)) * (XYMaxAngle - XYMinAngle);

    GLfloat XZMaxAngle = 45.0f;
    GLfloat XZMinAngle = -45.0f;
    GLfloat XZTargetAngle = XZMinAngle + ((x - mouseXMin) / (mouseXMax - mouseXMin)) * (XZMaxAngle - XZMinAngle);
    
    if (gXYArmAngle < XYTargetAngle) {
        gXYArmAngle += gArmSpeed * timeDifference;
    } else if (gXYArmAngle > XYTargetAngle) {
        gXYArmAngle -= gArmSpeed * timeDifference;
    }

    if (gXZArmAngle < XZTargetAngle) {
        gXZArmAngle += gArmSpeed * timeDifference;
    } else if (gXZArmAngle > XZTargetAngle) {
        gXZArmAngle -= gArmSpeed * timeDifference;
    }
}


void Player::SetXDirection(GLint xDirection) {
    gMovementDirection = xDirection;
}


void Player::SetYDirection(GLint yDirection) {
    gYDirection = yDirection;
}


GLfloat Player::GetFrontThighAngle() {
    return gFrontThighAngle;
}


GLfloat Player::GetBackThighAngle() {
    return gBackThighAngle;
}


GLfloat Player::GetFrontShinAngle() {
    return gFrontShinAngle;
}


GLfloat Player::GetBackShinAngle() {
    return gBackShinAngle;
}


void Player::RotateFrontThigh(GLfloat angle, GLdouble timeDifference) {
    gFrontThighAngle += angle * 0.1f * timeDifference;
}


void Player::RotateBackThigh(GLfloat angle, GLdouble timeDifference) {
    gBackThighAngle += angle * 0.1f * timeDifference;
}


void Player::RotateFrontShin(GLfloat angle, GLdouble timeDifference) {
    gFrontShinAngle += angle * 0.1f * timeDifference;
}


void Player::RotateBackShin(GLfloat angle, GLdouble timeDifference) {
    gBackShinAngle += angle * 0.1f * timeDifference;
}


void Player::SetFrontShinAngle(GLfloat angle) {
    gFrontShinAngle = angle;
}


void Player::SetBackShinAngle(GLfloat angle) {
    gBackShinAngle = angle;
}


// Aux rotation func
void Player::RotatePoint(GLfloat x, GLfloat y, GLfloat z, GLfloat XYangle, GLfloat XZangle, GLfloat YZangle, GLfloat &xOut, GLfloat &yOut, GLfloat &zOut) {
    // Rotation matrix
    GLfloat rotMatrix1[4][4] = {
        {cos(XYangle), -sin(XYangle), 0, 0},
        {sin(XYangle), cos(XYangle) , 0, 0},
        {0           , 0            , 1, 0},
        {0           , 0            , 0, 1},
    };

    GLfloat rotMatrix2[4][4] = {
        {1, 0           , 0            , 0},
        {0, cos(XZangle), -sin(XZangle), 0},
        {0, sin(XZangle), cos(XZangle) , 0},
        {0, 0           , 0            , 1},
    };

    GLfloat rotMatrix3[4][4] = {
        {cos(YZangle), 0, sin(YZangle), 0},
        {0           , 1, 0           , 0},
        {-sin(YZangle), 0, cos(YZangle), 0},
        {0           , 0, 0           , 1},
    };

    // Multiply rotMatrix2 by rotMatrix3
    GLfloat middleRotMatrix[4][4] = {0};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                middleRotMatrix[i][j] += rotMatrix2[i][k] * rotMatrix3[k][j];
            }
        }
    }

    // Multiply rotMatrix1 by middleRotMatrix
    GLfloat rotMatrix[4][4] = {0};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                rotMatrix[i][j] += rotMatrix1[i][k] * middleRotMatrix[k][j];
            }
        }
    }

    // Input vector
    GLfloat inVector[4] = {x, y, z, 1};

    // Output vector
    GLfloat outVector[4] = {0, 0, 0, 0};

    // Multiply rotMatrix by vector
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            outVector[i] += rotMatrix[i][j] * inVector[j];
        }
    }

    // Output values
    xOut = outVector[0];
    yOut = outVector[1];
    zOut = outVector[2];
}


// Aux translate func
void Player::TranslatePoint(GLfloat x, GLfloat y, GLfloat z, GLfloat dx, GLfloat dy, GLfloat dz, GLfloat &xOut, GLfloat &yOut, GLfloat &zOut) {
    // Translation matrix
    GLfloat transMatrix[4][4] = {
        {1, 0, 0, dx},
        {0, 1, 0, dy},
        {0, 0, 1, dz},
        {0, 0, 0, 1 },
    };

    // Input vector
    GLfloat inVector[4] = {x, y, z, 1};

    // Output vector
    GLfloat outVector[4] = {0, 0, 0, 0};

    // Multiply matrix by vector
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            outVector[i] += transMatrix[i][j] * inVector[j];
        }
    }

    // Output values
    xOut = outVector[0];
    yOut = outVector[1];
    zOut = outVector[2];
}


// Aux scale func
void Player::ScalePoint(GLfloat x, GLfloat y, GLfloat z, GLfloat sx, GLfloat sy, GLfloat sz, GLfloat &xOut, GLfloat &yOut, GLfloat &zOut) {
    // Sacle matriz
    GLfloat scaleMatrix[4][4] = {
        {sx, 0 , 0 , 0},
        {0 , sy, 0 , 0},
        {0 , 0 , sz, 0},
        {0 , 0 , 0 , 1},
    };

    // Input vector
    GLfloat inVector[4] = {x, y, z, 1};

    // Output vector
    GLfloat outVector[4] = {0, 0, 0, 0};

    // Multiply matrix by vector
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            outVector[i] += scaleMatrix[i][j] * inVector[j];
        }
    }

    // Output values
    xOut = outVector[0];
    yOut = outVector[1];
    zOut = outVector[2];
}


Shot* Player::Shoot(GLfloat maxDist) {
    GLfloat xBaseArm = 0.0, yBaseArm = 0.0, zBaseArm = 0.0;
    GLfloat xTopArm = 0.0, yTopArm = 0.0, zTopArm = 0.0;

    glPushMatrix();
        // Getting top position
        GLfloat x = 0.0, y = 0.0, z = 0.0;
        GLfloat xOut = 0.0, yOut = 0.0, zOut = 0.0;

        TranslatePoint(x, y, z, 0, gArmHeight, 0, xOut, yOut, zOut);
        x = xOut; y = yOut, z = zOut;

        RotatePoint(x, y, z, (gXYArmAngle * M_PI / 180), (gXZArmAngle * M_PI / 180), 0, xOut, yOut, zOut);
        x = xOut; y = yOut, z = zOut;

        TranslatePoint(x, y, z, 0, gBodyHeight/2, gBodyThickness/2, xOut, yOut, zOut);
        x = xOut; y = yOut, z = zOut;

        RotatePoint(x, y, z, 0, 0, (gXZAngle * M_PI / 180), xOut, yOut, zOut);
        x = xOut; y = yOut, z = zOut;

        TranslatePoint(x, y, z, gX, gY, gZ, xOut, yOut, zOut);
        x = xOut; y = yOut, z = zOut;
        

        xTopArm = x;
        yTopArm = y;
        zTopArm = z;


        // Getting base position
        x = 0.0; y = 0.0, z = 0.0;
        xOut = 0.0, yOut = 0.0, zOut = 0.0;

        RotatePoint(x, y, z, (gXYArmAngle * M_PI / 180), (gXZArmAngle * M_PI / 180), 0, xOut, yOut, zOut);
        x = xOut; y = yOut, z = zOut;

        TranslatePoint(x, y, z, 0, gBodyHeight/2, gBodyThickness/2, xOut, yOut, zOut);
        x = xOut; y = yOut, z = zOut;

        RotatePoint(x, y, z, 0, 0, (gXZAngle * M_PI / 180), xOut, yOut, zOut);
        x = xOut; y = yOut, z = zOut;
        
        TranslatePoint(x, y, z, gX, gY, gZ, xOut, yOut, zOut);
        x = xOut; y = yOut, z = zOut;
        

        xBaseArm = x;
        yBaseArm = y;
        zBaseArm = z;


        // Findig the direction
        GLfloat baseVector[3] = {xBaseArm, yBaseArm, zBaseArm};
        GLfloat topVector[3] = {xTopArm, yTopArm, zTopArm};

        // Now we have to consider 3 dimensions
        GLfloat xResVector = topVector[0] - baseVector[0];
        GLfloat yResVector = topVector[1] - baseVector[1];
        GLfloat zResVector = topVector[2] - baseVector[2];

        GLfloat norm = sqrt(pow(xResVector, 2) + pow(yResVector, 2) + pow(zResVector, 2));
        xResVector /= norm;
        yResVector /= norm;
        zResVector /= norm;

        GLfloat shotDirection[3] = {xResVector, yResVector, zResVector};
    glPopMatrix();

    return new Shot(xTopArm, yTopArm, zTopArm, gXZSpeed, shotDirection, maxDist, gBaseCircleRadius/10);
}


GLfloat Player::GetXSpeed() {
    return gXZSpeed;
}


GLfloat Player::GetYSpeed() {
    return gYSpeed;
}


GLint Player::GetXDirection() {
    return gMovementDirection;
}


GLint Player::GetYDirection() {
    return gYDirection;
}


void Player::Jump() {
    gYDirection = 1;
    jumpHeight = 0; // Reset jump height
}


GLfloat Player::GetMaxJumpHeight() {
    return maxJumpHeight;
}


GLfloat Player::GetJumpHeight() {
    return jumpHeight;
}


GLfloat Player::GetThighHeight() {
    return gThighHeight;
}


GLfloat Player::GetShinHeight() {
    return gShinHeight;
}


bool Player::ReachedMaximumJumpHeight() {
    return jumpHeight >= maxJumpHeight;
}


GLfloat Player::GetInvisibleReactHeight() {
    return gInvisibleReactHeight;
}


GLfloat Player::GetInvisibleReactWidth() {
    return gInvisibleReactWidth;
}


void Player::AnimateLegs(GLdouble timeDifference) {
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


GLfloat Player::GetXZAngle() {
    return gXZAngle;
}
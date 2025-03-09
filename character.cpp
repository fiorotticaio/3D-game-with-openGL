#include "character.h"
#include "text.h"

void Character::DrawHeadAndArms(GLfloat R, GLfloat G, GLfloat B) {
    glPushMatrix();
        // Draw the head
        glTranslatef(0, gBodyHeight + gHeadCircleRadius, 0);
        DrawSphereHead(gHeadCircleRadius, R, G, B);

        // Draw front arm (the one that moves)
        glTranslatef(0, -(gHeadCircleRadius + (gBodyHeight/2)), gBodyThickness/2);

        glPushMatrix(); // The angle dont interfear in the angle of the other arm
            glRotatef(gXYArmAngle, 0, 0, 1);
            glRotatef(gXZArmAngle, 1, 0, 0);
            DrawCuboid(gArmWidth, gArmHeight, gArmThickness, R, G, B);
        glPopMatrix();

        // Draw back arm (the one that doesn't move)
        glTranslatef(0, 0, -gBodyThickness);
        glRotatef(-150, 0, 0, 1); // Fixed angle
        DrawCuboid(gArmWidth, gArmHeight, gArmThickness, R, G, B);
    glPopMatrix();
}


void Character::DrawSphereHead(GLfloat radius, GLfloat R, GLfloat G, GLfloat B) {
    static bool texturesLoaded = false;
    static GLuint playerTexture, opponentTexture;

    if (!texturesLoaded) {
        playerTexture = LoadTextureRAW("textures/head-player.bmp");
        opponentTexture = LoadTextureRAW("textures/head-opponent.bmp");
        texturesLoaded = true;
    }

    GLfloat materialEmission[] = { 0.00, 0.00, 0.00, 1.0 };
    GLfloat materialColor[] = { R, G, B, 1.0 };
    GLfloat mat_diffuse[] = { 0.5, 0.5, 0.5, 1.0 };
    GLfloat mat_specular[] = { 0, 0, 0, 1.0 };
    GLfloat mat_shininess[] = { 0 };

    glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialColor);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glColor3f(R, G, B);

    GLUquadric* quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D, gIsPlayer ? playerTexture : opponentTexture);
    gluSphere(quad, radius, 20, 20);
    gluDeleteQuadric(quad);
    glBindTexture(GL_TEXTURE_2D, 0);
}


void Character::DrawFrontLeg(GLfloat R, GLfloat G, GLfloat B) {
    glPushMatrix();
        // Draw the front thigh
        glTranslatef(0, 0, -gBodyThickness/2);
        glRotatef(gFrontThighAngle, 0, 0, 1);
        DrawCuboid(gThighWidth, gThighHeight, gThighThickness, R, G, B);

        // Draw the front leg
        glTranslatef(0, gThighHeight, 0);
        glRotatef(gFrontShinAngle, 0, 0, 1);
        DrawCuboid(gShinWidth, gShinHeight, gShinThickness, R, G, B);
    glPopMatrix();
}


void Character::DrawBackLeg(GLfloat R, GLfloat G, GLfloat B) {
    glPushMatrix();
        // Draw the back thigh
        glTranslatef(0, 0, gBodyThickness/2);
        glRotatef(gBackThighAngle, 0, 0, 1);
        DrawCuboid(gThighWidth, gThighHeight, gThighThickness, R, G, B);

        // Draw the back shin
        glTranslatef(0, gThighHeight, 0);
        glRotatef(gBackShinAngle, 0, 0, 1);
        DrawCuboid(gShinWidth, gShinHeight, gShinThickness, R, G, B);
    glPopMatrix();
}


void Character::DrawCuboid(GLfloat width, GLfloat height, GLfloat depth, GLfloat R, GLfloat G, GLfloat B) {
    GLfloat materialEmission[] = { 0.00, 0.00, 0.00, 1.0 };
    GLfloat materialColor[] = { R, G, B, 1.0 };
    GLfloat mat_diffuse[] = { 0.5, 0.5, 0.5, 1.0 };
    GLfloat mat_specular[] = { 0.5, 0.5, 0.5, 1.0 };
    GLfloat mat_shininess[] = { 100 };
    
    glBegin(GL_QUADS);
        glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
        glMaterialfv(GL_FRONT, GL_AMBIENT, materialColor);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
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


GLfloat Character::GetGx() {
    return gX;
}


GLfloat Character::GetGy() {
    return gY;
}


void Character::SetGy(GLfloat y){
    gY = y;
}


GLfloat Character::GetGz() {
    return gZ;
}


void Character::MoveInXZ(GLfloat minPlayerPositionX, GLfloat maxPlayerPositionX, GLfloat minPlayerPositionZ, GLfloat maxPlayerPositionZ, GLdouble timeDifference) {
    GLfloat angleRad = gXZAngle * M_PI / 180.0f;
    GLfloat gXPreview = gX + gXZSpeed * timeDifference * gMovementDirection * cos(angleRad);
    GLfloat gZPreview = gZ - (gXZSpeed * timeDifference * gMovementDirection * sin(angleRad));

    bool playerViolatesXMIN = gXPreview <= minPlayerPositionX + gBodyWidth/2;
    bool playerViolatesXMAX = gXPreview >= maxPlayerPositionX - gBodyWidth/2;
    bool playerViolatesZMIN = gZPreview <= minPlayerPositionZ + gBodyWidth/2;
    bool playerViolatesZMAX = gZPreview >= maxPlayerPositionZ - gBodyWidth/2;

    if (!playerViolatesXMIN && !playerViolatesXMAX && !playerViolatesZMIN && !playerViolatesZMAX) {
        AnimateLegs(timeDifference);
        gX = gXPreview;
        gZ = gZPreview;
    }
}


void Character::MoveInY(GLfloat minPlayerPositionY, GLfloat maxPlayerPositionY, GLdouble timeDifference) {
    if (gY + gYSpeed * timeDifference * gYDirection >= minPlayerPositionY + gThighHeight + gShinHeight && 
        gY + gYSpeed * timeDifference * gYDirection <= maxPlayerPositionY - gThighHeight - gShinHeight) {
        gY += gYSpeed * timeDifference * gYDirection;
        jumpHeight += gYSpeed * timeDifference * gYDirection;
    }
}


void Character::Rotate(bool clockwise, GLdouble timeDifference) {
    gXZAngle += gRotationSpeed * (clockwise ? 1 : -1) * timeDifference; 
    
    // Keep the angle between 0 and 360
    if (gXZAngle > 360.0f) gXZAngle -= 360.0f;
    if (gXZAngle < 0.0f) gXZAngle += 360.0f;
}


void Character::RotateArm(GLfloat x, GLfloat y, GLfloat windowWidth, GLfloat windowHeight, GLdouble timeDifference) {
    GLfloat mouseXMin = 0;
    GLfloat mouseXMax = windowWidth;
    GLfloat mouseYMin = 0;
    GLfloat mouseYMax = windowHeight;

    GLfloat XYMaxAngle = -45.0f;
    GLfloat XYMinAngle = -135.0f;
    GLfloat XYTargetAngle = XYMinAngle + ((y - mouseYMin) / (mouseYMax - mouseYMin)) * (XYMaxAngle - XYMinAngle);

    GLfloat XZMaxAngle = 45.0f;
    GLfloat XZMinAngle = -45.0f;
    GLfloat XZTargetAngle = XZMinAngle + ((x - mouseXMin) / (mouseXMax - mouseXMin)) * (XZMaxAngle - XZMinAngle);

    GLfloat tolerance = 0.5f;

    // Atualiza XYArmAngle sem ultrapassar o alvo
    if (fabs(gXYArmAngle - XYTargetAngle) > tolerance) {
        gXYArmAngle += std::min(static_cast<GLdouble>(gArmSpeed * timeDifference), static_cast<GLdouble>(fabs(gXYArmAngle - XYTargetAngle))) * (gXYArmAngle < XYTargetAngle ? 1 : -1);
    }

    // Atualiza XZArmAngle sem ultrapassar o alvo
    if (fabs(gXZArmAngle - XZTargetAngle) > tolerance) {
        gXZArmAngle += std::min(static_cast<GLdouble>(gArmSpeed * timeDifference), static_cast<GLdouble>(fabs(gXZArmAngle - XZTargetAngle))) * (gXZArmAngle < XZTargetAngle ? 1 : -1);
    }
}


void Character::SetMovementDirection(GLint direction) {
    gMovementDirection = direction;
}


void Character::SetYDirection(GLint yDirection) {
    gYDirection = yDirection;
}


GLfloat Character::GetFrontThighAngle() {
    return gFrontThighAngle;
}


GLfloat Character::GetBackThighAngle() {
    return gBackThighAngle;
}


GLfloat Character::GetFrontShinAngle() {
    return gFrontShinAngle;
}


GLfloat Character::GetBackShinAngle() {
    return gBackShinAngle;
}


void Character::RotateFrontThigh(GLfloat angle, GLdouble timeDifference) {
    gFrontThighAngle += angle * 0.1f * timeDifference;
}


void Character::RotateBackThigh(GLfloat angle, GLdouble timeDifference) {
    gBackThighAngle += angle * 0.1f * timeDifference;
}


void Character::RotateFrontShin(GLfloat angle, GLdouble timeDifference) {
    gFrontShinAngle += angle * 0.1f * timeDifference;
}


void Character::RotateBackShin(GLfloat angle, GLdouble timeDifference) {
    gBackShinAngle += angle * 0.1f * timeDifference;
}


void Character::SetFrontShinAngle(GLfloat angle) {
    gFrontShinAngle = angle;
}


void Character::SetBackShinAngle(GLfloat angle) {
    gBackShinAngle = angle;
}


// Aux rotation func
void Character::RotatePoint(GLfloat x, GLfloat y, GLfloat z, GLfloat XYangle, GLfloat XZangle, GLfloat YZangle, GLfloat &xOut, GLfloat &yOut, GLfloat &zOut) {
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
void Character::TranslatePoint(GLfloat x, GLfloat y, GLfloat z, GLfloat dx, GLfloat dy, GLfloat dz, GLfloat &xOut, GLfloat &yOut, GLfloat &zOut) {
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
void Character::ScalePoint(GLfloat x, GLfloat y, GLfloat z, GLfloat sx, GLfloat sy, GLfloat sz, GLfloat &xOut, GLfloat &yOut, GLfloat &zOut) {
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


Shot* Character::Shoot(GLfloat maxDist) {
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


GLfloat Character::GetXZSpeed() {
    return gXZSpeed;
}

void Character::setXZSpeed(GLfloat speed){
    gXZSpeed = speed;
}


GLfloat Character::GetYSpeed() {
    return gYSpeed;
}


GLint Character::GetMovementDirection(){
    return gMovementDirection;
}


GLint Character::GetYDirection() {
    return gYDirection;
}


void Character::Jump() {
    gYDirection = 1;
    jumpHeight = 0; // Reset jump height
}


GLfloat Character::GetMaxJumpHeight() {
    return maxJumpHeight;
}


GLfloat Character::GetJumpHeight() {
    return jumpHeight;
}


GLfloat Character::GetThighHeight() {
    return gThighHeight;
}


GLfloat Character::GetShinHeight() {
    return gShinHeight;
}


bool Character::ReachedMaximumJumpHeight() {
    return jumpHeight >= maxJumpHeight;
}


GLfloat Character::GetHitboxHeight() {
    return gHitboxHeight;
}


GLfloat Character::GetHitboxRadius() {
    return gHitboxRadius;
}


void Character::AnimateLegs(GLdouble timeDifference) {
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


GLfloat Character::GetXZAngle() {
    return gXZAngle;
}


GLfloat Character::GetXZArmAngle() {
    return gXZArmAngle;
}


GLfloat Character::GetXYArmAngle() {
    return gXYArmAngle;
}


void Character::CalculateArmTopPos(GLfloat* armTopPos) {
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
    glPopMatrix();

    armTopPos[0] = xTopArm;
    armTopPos[1] = yTopArm;
    armTopPos[2] = zTopArm;
}


void Character::CalculateArmLookAt(GLfloat* armLookAt) {
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

    armLookAt[0] = shotDirection[0];
    armLookAt[1] = shotDirection[1];
    armLookAt[2] = shotDirection[2];
}


void Character::CalculateGunSightPos(GLfloat* gunSightPos) {
    GLfloat xGunSight = 0.0, yGunSight = 0.0, zGunSight = 0.0;

    glPushMatrix();
        // Getting top position
        GLfloat x = 0.0, y = 0.0, z = 0.0;
        GLfloat xOut = 0.0, yOut = 0.0, zOut = 0.0;

        TranslatePoint(x, y, z, -gArmThickness*2.2, 0, 0, xOut, yOut, zOut);
        x = xOut; y = yOut, z = zOut;

        TranslatePoint(x, y, z, 0, gArmHeight/8, 0, xOut, yOut, zOut);
        x = xOut; y = yOut, z = zOut;
        
        RotatePoint(x, y, z, (gXYArmAngle * M_PI / 180), (gXZArmAngle * M_PI / 180), 0, xOut, yOut, zOut);
        x = xOut; y = yOut, z = zOut;
        
        TranslatePoint(x, y, z, 0, gBodyHeight/2, gBodyThickness/2, xOut, yOut, zOut);
        x = xOut; y = yOut, z = zOut;
        
        RotatePoint(x, y, z, 0, 0, (gXZAngle * M_PI / 180), xOut, yOut, zOut);
        x = xOut; y = yOut, z = zOut;
        
        TranslatePoint(x, y, z, gX, gY, gZ, xOut, yOut, zOut);
        x = xOut; y = yOut, z = zOut;

        xGunSight = x;
        yGunSight = y;
        zGunSight = z;
    glPopMatrix();

    gunSightPos[0] = xGunSight;
    gunSightPos[1] = yGunSight;
    gunSightPos[2] = zGunSight;
}


void Character::CalculateFlashlightPos(GLfloat* flashlightPos) {
    GLfloat xflashlight = 0.0, yflashlight = 0.0, zflashlight = 0.0;

    glPushMatrix();
        // Getting top position
        GLfloat x = 0.0, y = 0.0, z = 0.0;
        GLfloat xOut = 0.0, yOut = 0.0, zOut = 0.0;

        TranslatePoint(x, y, z, -gArmThickness, 0, 0, xOut, yOut, zOut);
        x = xOut; y = yOut, z = zOut;

        TranslatePoint(x, y, z, 0, gArmHeight/1.5, 0, xOut, yOut, zOut);
        x = xOut; y = yOut, z = zOut;
        
        RotatePoint(x, y, z, (gXYArmAngle * M_PI / 180), (gXZArmAngle * M_PI / 180), 0, xOut, yOut, zOut);
        x = xOut; y = yOut, z = zOut;
        
        TranslatePoint(x, y, z, 0, gBodyHeight/2, gBodyThickness/2, xOut, yOut, zOut);
        x = xOut; y = yOut, z = zOut;
        
        RotatePoint(x, y, z, 0, 0, (gXZAngle * M_PI / 180), xOut, yOut, zOut);
        x = xOut; y = yOut, z = zOut;
        
        TranslatePoint(x, y, z, gX, gY, gZ, xOut, yOut, zOut);
        x = xOut; y = yOut, z = zOut;

        xflashlight = x;
        yflashlight = y;
        zflashlight = z;
    glPopMatrix();

    flashlightPos[0] = xflashlight;
    flashlightPos[1] = yflashlight;
    flashlightPos[2] = zflashlight;
}


void Character::DrawHitbox() {
    glPushMatrix();
        glTranslatef(gX, gY - gThighHeight - gShinHeight, gZ);
        glColor3f(1.0f, 1.0f, 1.0f);
        glPointSize(100);

        // Bottom
        glBegin(GL_POLYGON);
            for (int i = 0; i < 20; i++) glVertex3f(gHitboxRadius * cos(i * 2 * M_PI / 20), 0, gHitboxRadius * sin(i * 2 * M_PI / 20));
        glEnd();

        // Top
        glTranslatef(0, gHitboxHeight, 0);
        glBegin(GL_POLYGON);
        for (int i = 0; i < 20; i++) glVertex3f(gHitboxRadius * cos(i * 2 * M_PI / 20), 0, gHitboxRadius * sin(i * 2 * M_PI / 20));
        glEnd();

        // Line connecting 
        glBegin(GL_LINES);
            // Front line
            glVertex3f(gHitboxRadius, 0, 0);
            glVertex3f(gHitboxRadius, -gHitboxHeight, 0);

            // Right line
            glVertex3f(0, 0, gHitboxRadius);
            glVertex3f(0, -gHitboxHeight, gHitboxRadius);

            // Back line
            glVertex3f(-gHitboxRadius, 0, 0);
            glVertex3f(-gHitboxRadius, -gHitboxHeight, 0);

            // Left line
            glVertex3f(0, 0, -gHitboxRadius);
            glVertex3f(0, -gHitboxHeight, -gHitboxRadius);
        glEnd();

    glPopMatrix();

    char pos[50];
    sprintf(pos, "Position: (x: %.2f, y: %.2f, z: %.2f)", gX, gY, gZ);
    PrintTextWorld(gX, gY + gHitboxHeight, gZ, pos, 1, 1, 1);

    GLfloat charLeftX = gX - gHitboxRadius;
    GLfloat charRightX = gX + gHitboxRadius;
    GLfloat charBottomY = gY - gThighHeight - gShinHeight;
    GLfloat charTopY = charBottomY + gHitboxHeight;
    GLfloat charFrontZ = gZ + gHitboxRadius;
    GLfloat charBackZ = gZ - gHitboxRadius;
    char hitbox[100];
    sprintf(hitbox, "Hitbox: (X: %.2f, %.2f) (Y: %.2f, %.2f)", charLeftX, charRightX, charBottomY, charTopY);
    PrintTextWorld(gX, gY + gHitboxHeight + 1.0, gZ, hitbox, 1, 1, 1);

    char hitboxCont[100];
    sprintf(hitboxCont, "(Z: %.2f, %.2f)", charBackZ, charFrontZ);
    PrintTextWorld(gX, gY + gHitboxHeight + 0.5, gZ, hitboxCont, 1, 1, 1);
}
#ifndef CHARACTER_H
#define CHARACTER_H



#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <stdio.h>
#include <random>
#include "shot.h"


class Character {
protected:
    GLfloat gX;
    GLfloat gY;
    GLfloat gZ;

    GLfloat gBaseCircleRadius; // Read in the svg file
    GLfloat gHeadCircleRadius;
    GLfloat gHeadAngle;

    GLfloat gBodyWidth;
    GLfloat gBodyHeight;
    GLfloat gBodyThickness;

    GLfloat gHitboxRadius;
    GLfloat gHitboxHeight;

    GLfloat gArmWidth;
    GLfloat gArmHeight;
    GLfloat gArmThickness;
    GLfloat gXZArmAngle;
    GLfloat gXYArmAngle;
    GLfloat gArmSpeed;

    GLfloat gThighWidth;
    GLfloat gThighHeight;
    GLfloat gThighThickness;
    GLfloat gFrontThighAngle;
    GLfloat gBackThighAngle;

    GLfloat gShinWidth;
    GLfloat gShinHeight;
    GLfloat gShinThickness;
    GLfloat gFrontShinAngle;
    GLfloat gBackShinAngle;

    GLint gFrontThighAngleDir;
    GLint gBackThighAngleDir;
    GLint gFrontShinAngleDir;
    GLint gBackShinAngleDir;

    GLfloat gXZSpeed;
    GLfloat gYSpeed;
    GLint gMovementDirection; // 1 for right (front), -1 for left (back)
    GLint gYDirection; // 1 for up, -1 for down
    GLfloat gXZAngle;
    GLfloat gRotationSpeed;

    GLfloat maxJumpHeight;
    GLfloat jumpHeight;
    bool gIsPlayer;

    void DrawCuboid(GLfloat width, GLfloat height, GLfloat depth, GLfloat R, GLfloat G, GLfloat B);
    void DrawSphereHead(GLfloat radius, GLfloat R, GLfloat G, GLfloat B);
    void DrawHeadAndArms(GLfloat R, GLfloat G, GLfloat B);
    void DrawFrontLeg(GLfloat R, GLfloat G, GLfloat B);
    void DrawBackLeg(GLfloat R, GLfloat G, GLfloat B);
    void RotatePoint(GLfloat x, GLfloat y, GLfloat z, GLfloat XYangle, GLfloat XZangle, GLfloat YZangle, GLfloat &xOut, GLfloat &yOut, GLfloat &zOut);
    void TranslatePoint(GLfloat x, GLfloat y, GLfloat z, GLfloat dx, GLfloat dy, GLfloat dz, GLfloat &xOut, GLfloat &yOut, GLfloat &zOut);
    void ScalePoint(GLfloat x, GLfloat y, GLfloat z, GLfloat sx, GLfloat sy, GLfloat sz, GLfloat &xOut, GLfloat &yOut, GLfloat &zOut);


public:
    Character(GLfloat x, GLfloat y, GLfloat baseCircleRadius, GLfloat arenaThickness) {
        gX = x;
        gY = y;
        
        // Random number generator with uniform distribution between -1 and -(arenaThickness-1)
        std::random_device rd;  
        std::mt19937 gen(rd()); 
        std::uniform_real_distribution<GLfloat> dist(-(arenaThickness-1), -1.0f);
        gZ = dist(gen);

        gMovementDirection = 1;
        gYDirection = -1;
        gXZAngle = 0;
        gBaseCircleRadius = baseCircleRadius;
        gHeadCircleRadius = ((float) 22 / (float) 172) * gBaseCircleRadius;

        gBodyHeight = ((float) 53 / 172) * gBaseCircleRadius;
        gBodyWidth = (float) gBodyHeight / (float) 2;
        gBodyThickness = gBodyWidth;

        gArmHeight = ((float) 57 / (float) 172) * gBaseCircleRadius;
        gArmWidth = (float) gArmHeight / (float) 6;
        gArmThickness = gArmWidth;
        gXZArmAngle = 0.0f;
        gXYArmAngle = -90.0f;
        gArmSpeed = 0.5f;

        gThighHeight = ((float) 47 / (float) 172) * gBaseCircleRadius;
        gThighWidth = (float) gThighHeight / (float) 6;
        gThighThickness = gThighWidth;
        gFrontThighAngle = -140.0f;
        gBackThighAngle = -210.0f;

        gShinHeight = ((float) 50 / (float) 172) * gBaseCircleRadius;
        gShinWidth = (float) gShinHeight / (float)  6;
        gShinThickness = gShinWidth;
        gFrontShinAngle = 0.0f;
        gBackShinAngle = -50.0f;
        gXZSpeed = 0.03f;
        gYSpeed = 0.025f;
        gRotationSpeed = 0.15f;

        gHitboxHeight = gThighHeight + gShinHeight + gBodyHeight + 2 * gHeadCircleRadius;
        gHitboxRadius = gBodyWidth;

        maxJumpHeight = 5 * gHitboxHeight; // 4 instead of 3 to make the game more playable
        jumpHeight = 0;

        gFrontThighAngleDir = 1;
        gBackThighAngleDir = 1;
        gFrontShinAngleDir = -1;
        gBackShinAngleDir = -1;

        gIsPlayer = false;
    }

    virtual void Draw() = 0;
    virtual ~Character() {}

    GLfloat GetGx();
    GLfloat GetGy();
    void SetGy(GLfloat y);
    GLfloat GetGz();
    void MoveInXZ(GLfloat minPlayerPositionX, GLfloat maxPlayerPositionX, GLfloat minPlayerPositionZ, GLfloat maxPlayerPositionZ, GLdouble timeDifference);
    void MoveInY(GLfloat minPlayerPositionY, GLfloat maxPlayerPositionY, GLdouble timeDifference);
    void Rotate(bool clockwise, GLdouble timeDifference);
    void RotateArm(GLfloat x, GLfloat y, GLfloat windowWidth, GLfloat windowHeight, GLdouble timeDifference);
    GLint GetMovementDirection();
    void SetMovementDirection(GLint direction);
    void SetYDirection(GLint yDirection);
    GLint GetYDirection();
    GLfloat GetFrontThighAngle();
    GLfloat GetBackThighAngle();
    GLfloat GetFrontShinAngle();
    GLfloat GetBackShinAngle();
    void RotateFrontThigh(GLfloat angle, GLdouble timeDifference);
    void RotateBackThigh(GLfloat angle, GLdouble timeDifference);
    void RotateFrontShin(GLfloat angle, GLdouble timeDifference);
    void RotateBackShin(GLfloat angle, GLdouble timeDifference);
    void SetFrontShinAngle(GLfloat angle);
    void SetBackShinAngle(GLfloat angle);
    Shot* Shoot(GLfloat maxDist);
    GLfloat GetXZSpeed();
    void setXZSpeed(GLfloat speed);
    GLfloat GetYSpeed();
    void Jump();
    GLfloat GetMaxJumpHeight();
    GLfloat GetJumpHeight();
    GLfloat GetThighHeight();
    GLfloat GetShinHeight();
    bool ReachedMaximumJumpHeight();
    GLfloat GetHitboxHeight();
    GLfloat GetHitboxRadius();
    void AnimateLegs(GLdouble timeDifference);
    GLfloat GetXZAngle();
    GLfloat GetXZArmAngle();
    GLfloat GetXYArmAngle();
    void CalculateArmTopPos(GLfloat* armTopPos);
    void CalculateArmLookAt(GLfloat* armLookAt);
    void CalculateGunSightPos(GLfloat* gunSightPos);
    void CalculateFlashlightPos(GLfloat* flashlightPos);
    void DrawHitbox();
};



#endif // CHARACTER_H
#ifndef OPPONENT_H
#define OPPONENT_H



#include "character.h"


class Opponent : public Character {
protected:
    bool gCollideWithRightX;
    bool gCollideWithLeftX;
    bool gCollideWithRightZ;
    bool gCollideWithLeftZ;


public:
    Opponent(GLfloat x, GLfloat y, GLfloat baseCircleRadius, GLfloat arenaThickness);
    
    void Draw() override;
    void RotateArmToTargetAngle(GLdouble timeDifference, GLfloat XZtargetAngle , GLfloat XYtargetAngle);
    bool GetCollideWithRightX();
    bool GetCollideWithLeftX();
    bool GetCollideWithRightZ();
    bool GetCollideWithLeftZ();
    void SetCollideWithRightX(bool collideWithRightX);
    void SetCollideWithLeftX(bool collideWithLeftX);
    void SetCollideWithRightZ(bool collideWithRightZ);
    void SetCollideWithLeftZ(bool collideWithLeftZ);
};



#endif // OPPONENT_H
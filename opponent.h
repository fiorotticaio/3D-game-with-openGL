#ifndef OPPONENT_H
#define OPPONENT_H



#include "character.h"


class Opponent : public Character {
public:
    Opponent(GLfloat x, GLfloat y, GLfloat baseCircleRadius, GLfloat arenaThickness);
    
    void Draw() override;
    void RotateArmToTargetAngle(GLdouble timeDifference, GLfloat XZtargetAngle , GLfloat XYtargetAngle);
};



#endif // OPPONENT_H
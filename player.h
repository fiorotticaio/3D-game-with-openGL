#ifndef PLAYER_H
#define PLAYER_H



#include "character.h"


class Player : public Character {
public:
    Player(GLfloat x, GLfloat y, GLfloat baseCircleRadius, GLfloat arenaThickness);

    void Draw() override;
};



#endif // PLAYER_H
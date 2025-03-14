#ifndef PLAYER_H
#define PLAYER_H



#include "character.h"
#include "imageloader.h"


class Player : public Character {
protected:
    GLuint gTShirtFrontTexture;
    GLuint gTShirtBaseTexture;
    GLuint gTShirtBackTexture;

    
public:
    Player(GLfloat x, GLfloat y, GLfloat baseCircleRadius, GLfloat arenaThickness);
    
    void Draw() override;
};



#endif // PLAYER_H
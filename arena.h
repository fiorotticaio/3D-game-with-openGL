#ifndef ARENA_H
#define ARENA_H



#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "player.h"
#include "obstacle.h"
#include "opponent.h"
#include <vector>
#include "tinyxml2.h"
#include <string>
#include "shot.h"


class Arena {
    GLfloat gX;
    GLfloat gY;

    GLfloat gWidth;
    GLfloat gHeight;
    GLfloat gThickness;

    GLfloat gRed;
    GLfloat gGreen;
    GLfloat gBlue;
    
    Player* gPlayer;
    std::vector<Obstacle*> gObstacles;
    std::vector<Opponent*> gOpponents;

    GLuint gGroundTexture;
    GLuint gWallTexture;
    GLuint gRoofTexture;


private:
    void LoadArena(const char* svg_file_path);
    void DrawArena();
    void DrawRects(GLfloat width, GLfloat height, GLfloat thickness, GLfloat R, GLfloat G, GLfloat B);
    bool PlayerCollidesWithObstacle(Obstacle* obstacle, GLdouble timeDifference);
    bool PlayerLandsInObstacle(Obstacle* obstacle, GLdouble timeDifference);
    bool PlayerCollidesWithOpponent(Player* player, Opponent* opponent, GLfloat dx, GLfloat dy);
    bool PlayerLandsInOpponent(Opponent* opponent, GLdouble timeDifference);
    bool PlayerCollidesWithGround(Player* player, GLfloat dx, GLfloat dy);
    bool OpponentCollidesWithObstacle(Opponent* opponent, Obstacle* obstacle, GLfloat dx, GLfloat dy);
    bool OpponentLandsInObstacle(Opponent* opponent, Obstacle* obstacle, GLfloat dx, GLfloat dy);
    bool OpponentCollidesWithGround(Opponent* opponent, GLfloat dx, GLfloat dy);
    bool ObstacleCollidesWithShot(Obstacle* obstacle, Shot* shot);
    bool OpponentCollidesWithShot(Opponent* opponent, Shot* shot);


public:
    Arena(const char* svg_file_path, GLuint groundTexture, GLuint wallTexture) {
        LoadArena(svg_file_path);
        gGroundTexture = groundTexture;
        gWallTexture = wallTexture;
    }

    void Draw() {
        DrawArena();
    }

    GLfloat GetGx();
    GLfloat GetGy();
    GLfloat GetWidth();
    GLfloat GetHeight();
    GLfloat GetThickness();
    GLfloat GetPlayerGx();
    GLfloat GetPlayerGy();
    GLfloat GetPlayerGz();
    void MovePlayerInXZ(GLdouble timeDifference);
    void MovePlayerInY(GLdouble timeDifference);
    void RotatePlayer(bool clockwise, GLdouble timeDifference);
    void RotatePlayerArm(GLfloat x, GLfloat y, GLfloat windowWidth, GLfloat windowHeight, GLdouble timeDifference);
    void SetPlayerMovementDirection(GLint direction);
    GLint GetPlayerMovementDirection();
    void SetPlayerYDirection(GLint yDirection);
    GLint GetPlayerYDirection();
    GLfloat GetPlayerFrontThighAngle();
    GLfloat GetPlayerBackThighAngle();
    GLfloat GetPlayerFrontShinAngle();
    GLfloat GetPlayerBackShinAngle();
    void RotatePlayerFrontThigh(GLfloat angle, GLdouble timeDifference);
    void RotatePlayerBackThigh(GLfloat angle, GLdouble timeDifference);
    void RotatePlayerFrontShin(GLfloat angle, GLdouble timeDifference);
    void RotatePlayerBackShin(GLfloat angle, GLdouble timeDifference);
    void SetPlayerFrontShinAngle(GLfloat angle);
    void SetPlayerBackShinAngle(GLfloat angle);
    Shot* PlayerShoot(GLfloat maxDist);
    void PlayerJump();
    GLfloat GetPlayerMaxJumpHeight();
    GLfloat GetPlayerJumpHeight();
    GLfloat GetPlayerThighHeight();
    GLfloat GetPlayerShinHeight();
    bool PlayerReachedMaximumJumpHeight();
    bool PlayerLanded(GLdouble timeDifference);
    bool OpponentLanded(Opponent* opponent);
    std::vector<Obstacle*> GetObstacles();
    std::vector<Opponent*> GetOpponents();
    void EraseOpponent(Opponent* opponent);
    void MoveOpponentsInY(GLdouble timeDifference);
	void MoveOpponentsInXZ(GLdouble timeDifference);
    bool ObstaclesCollidesWithShot(Shot* shot);
    bool OpponentsCollidesWithShot(Shot* shot);
    void MoveOpponentsArms(GLdouble timeDifference);
    void UpdateOpponentsShots(std::vector<Shot*>& opponentsShots, GLfloat maxDist, GLdouble timeDifference);
    bool PlayerCollidesWithShot(Shot* shot);
    bool PlayerHitsHead();
    bool PlayerHitsHeadRoof(Player* player, GLfloat dx, GLfloat dy);
    bool PlayerHitsHeadObstacle(Player* player, Obstacle* obstacle, GLfloat dx, GLfloat dy);
    bool PlayerHitsHeadOpponent(Player* player, Opponent* opponent, GLfloat dx, GLfloat dy);
    bool PlayerWon();
    void Delete();
    void AnimatePlayerLegs(GLdouble timeDifference);
    void AnimateOpponentsLegs(GLdouble timeDifference);
    bool OpponentLandsInPlayer(Opponent* opponent, Player* player, GLfloat dx, GLfloat dy);
    bool OpponentCollidesWithOtherOpponent(Opponent* opponent, Opponent* otherOpponent, GLfloat dx, GLfloat dy);
    bool OpponentLandsInOtherOpponent(Opponent* opponent, Opponent* otherOpponent, GLfloat dx, GLfloat dy);
    GLfloat CalculatePlayerHeadYPosition();
    GLfloat GetPlayerXZAngle();
    GLfloat GetPlayerXZArmAngle();
    GLfloat GetPlayerXYArmAngle();
    void CalculatePlayerArmTopPos(GLfloat* playerArmTopPos);
    void CalculatePlayerArmLookAt(GLfloat* playerArmLookAt);
    void DrawHitboxes();
};



#endif // ARENA_H
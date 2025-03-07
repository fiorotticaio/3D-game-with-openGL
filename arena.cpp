#include "arena.h"

GLfloat MapYCoordinate(GLfloat yRead, GLfloat yPositionArena, GLfloat arenaHeight) {
    // The y coordenates of the svg are inverted
    return yPositionArena + arenaHeight - (yRead - yPositionArena);
}


void Arena::LoadArena(const char* svg_file_path) {
    using namespace tinyxml2;

    XMLDocument doc;
    if (doc.LoadFile(svg_file_path) != XML_SUCCESS) {
        printf("Error loading SVG file: %s\n", svg_file_path);
        exit(1);
    }

    XMLElement* root = doc.FirstChildElement("svg");
    if (!root) {
        printf("<svg> element not found in file\n");
        exit(1);
    }

    // Pass trough <rect> and <circle> elements
    for (XMLElement* elem = root->FirstChildElement(); elem; elem = elem->NextSiblingElement()) {
        std::string tag = elem->Name();
        
        if (tag == "rect") {
            const char* fill = elem->Attribute("fill");

            if (fill) {
                std::string fillStr = fill;
                if (fillStr == "blue") {
                    // Arena parameters
                    gWidth = elem->FloatAttribute("width");
                    gHeight = elem->FloatAttribute("height");
                    gThickness = gHeight / 2; 
                    gX = elem->FloatAttribute("x");
                    gY = elem->FloatAttribute("y");
                    gRed = 0.0f;
                    gGreen = 0.0f;
                    gBlue = 1.0f;

                } else if (fillStr == "black") {
                    // Obstacle parameters
                    GLfloat obstacleX = elem->FloatAttribute("x");
                    GLfloat obstacleY = elem->FloatAttribute("y");
                    GLfloat obstacleWidth = elem->FloatAttribute("width");
                    GLfloat obstacleHeight = elem->FloatAttribute("height");

                    obstacleY = MapYCoordinate(obstacleY, gY, gHeight);
                    
                    // TODO: Cases when an obstacle is defined in the svg before the arena
                    gObstacles.push_back(new Obstacle(obstacleX, obstacleY, obstacleWidth, obstacleHeight, gThickness));
                }
            }
        } else if (tag == "circle") {
            const char* fill = elem->Attribute("fill");

            if (fill) {
                std::string fillStr = fill;
                GLfloat cx = elem->FloatAttribute("cx");
                GLfloat cy = elem->FloatAttribute("cy");
                GLfloat radius = elem->FloatAttribute("r");

                cy = MapYCoordinate(cy, gY, gHeight);

                if (fillStr == "green") {
                    // TODO: Cases when an obstacle is defined in the svg before the arena
                    gPlayer = new Player(cx, cy, radius, gThickness);
                } else if (fillStr == "red") {
                    // TODO: Cases when an obstacle is defined in the svg before the arena
                    gOpponents.push_back(new Opponent(cx, cy, radius, gThickness));
                }
            }
        }
    }

    if (!gPlayer) {
        printf("Player (green circle) not found in SVG file.\n");
        exit(1);
    }
}


void Arena::DrawArena() {
    GLfloat materialEmission[] = { 0.00, 0.00, 0.00, 1.0 };
    GLfloat materialColor[] = { 0.0, 0.0, 0.8, 1.0 };
    GLfloat mat_diffuse[] = { 0.5, 0.5, 0.5, 1.0 };
    GLfloat mat_specular[] = { 0.5, 0.5, 0.5, 1.0 };
    GLfloat mat_shininess[] = { 50 };
    // GLfloat materialEmission[] = { 0.05, 0.05, 0.2, 1.0 };  // Pequeno brilho azul
    // GLfloat materialColor[] = { 0.2, 0.2, 0.9, 1.0 };       // Azul mais vibrante e iluminado
    // GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };         // Mantém boa resposta à luz
    // GLfloat mat_specular[] = { 0.3, 0.3, 0.3, 1.0 };        // Reflexos mais suaves
    // GLfloat mat_shininess[] = { 20 };                       // Reduz brilho especular

    glPushMatrix();
        glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
        glMaterialfv(GL_FRONT, GL_AMBIENT, materialColor);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
        glColor3f(0, 0, 1);

        glTranslatef(gX, gY, 0);
        DrawRects(gWidth, gHeight, gThickness, gRed, gGreen, gBlue);
    glPopMatrix();

    for (Obstacle* obstacle : gObstacles) {
        obstacle->Draw();
    }

    gPlayer->Draw();

    for (Opponent* opponent : gOpponents) {
        opponent->Draw();
    }
}


void Arena::DrawRects(GLfloat width, GLfloat height, GLfloat thickness, GLfloat R, GLfloat G, GLfloat B) {
    glColor3f(R, G, B);

    // The coordenates givem in the svg file are the left bottom corner of the arena

    // 1º Face
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, gWallTexture);
    double textureS = 10;
    glBegin(GL_QUADS);
        glNormal3f(0, 0, -1);
        glTexCoord2f(0, 0);
        glVertex3f(0, 0, 0);

        glNormal3f(0, 0, -1);
        glTexCoord2f(0, textureS);
        glVertex3f(0, height, 0);

        glNormal3f(0, 0, -1);
        glTexCoord2f(textureS, textureS);
        glVertex3f(width, height, 0);

        glNormal3f(0, 0, -1);
        glTexCoord2f(textureS, 0);
        glVertex3f(width, 0, 0);
    glEnd();

    // 2º Face
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, gWallTexture);
    textureS = 1;
    glBegin(GL_QUADS);
        glNormal3f(-1, 0, 0);
        glTexCoord2f(0, 0);
        glVertex3f(width, 0, 0);

        glNormal3f(-1, 0, 0);
        glTexCoord2f(0, textureS);
        glVertex3f(width, height, 0);

        glNormal3f(-1, 0, 0);
        glTexCoord2f(textureS, textureS);
        glVertex3f(width, height, -thickness);

        glNormal3f(-1, 0, 0);
        glTexCoord2f(textureS, 0);
        glVertex3f(width, 0, -thickness);
    glEnd();

    // 3º Face
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, gWallTexture);
    textureS = 10;
    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);
        glTexCoord2f(0, 0);
        glVertex3f(width, 0, -thickness);

        glNormal3f(0, 0, 1);
        glTexCoord2f(0, textureS);
        glVertex3f(width, height, -thickness);

        glNormal3f(0, 0, 1);
        glTexCoord2f(textureS, textureS);
        glVertex3f(0, height, -thickness);

        glNormal3f(0, 0, 1);
        glTexCoord2f(textureS, 0);
        glVertex3f(0, 0, -thickness);
    glEnd();

    // 4º Face
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, gWallTexture);
    textureS = 1;
    glBegin(GL_QUADS);
        glNormal3f(1, 0, 0);
        glTexCoord2f(0, 0);
        glVertex3f(0, 0, -thickness);

        glNormal3f(1, 0, 0);
        glTexCoord2f(0, textureS);
        glVertex3f(0, height, -thickness);

        glNormal3f(1, 0, 0);
        glTexCoord2f(textureS, textureS);
        glVertex3f(0, height, 0);

        glNormal3f(1, 0, 0);
        glTexCoord2f(textureS, 0);
        glVertex3f(0, 0, 0);
    glEnd();

    // Ground
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, gGroundTexture);
    textureS = 5;
    glBegin(GL_QUADS);
        glNormal3f(0, 1, 0);
        glTexCoord2f(0, 0);
        glVertex3f(0, 0, 0);

        glNormal3f(0, 1, 0);
        glTexCoord2f(textureS, 0);
        glVertex3f(width, 0, 0);
        
        glNormal3f(0, 1, 0);
        glTexCoord2f(textureS, textureS);
        glVertex3f(width, 0, -thickness);

        glNormal3f(0, 1, 0);
        glTexCoord2f(0, textureS);
        glVertex3f(0, 0, -thickness);
    glEnd();

    // Roof
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // glBindTexture(GL_TEXTURE_2D, gRoofTexture);
    // textureS = 1;
    glBegin(GL_QUADS);
        glNormal3f(0, -1, 0);
        // glTexCoord2f(0, 0);
        glVertex3f(0, height, 0);

        glNormal3f(0, -1, 0);
        // glTexCoord2f(0, textureS);
        glVertex3f(0, height, -thickness);

        glNormal3f(0, -1, 0);
        // glTexCoord2f(textureS, textureS);
        glVertex3f(width, height, -thickness);

        glNormal3f(0, -1, 0);
        // glTexCoord2f(textureS, 0);
        glVertex3f(width, height, 0);
    glEnd();
}


GLfloat Arena::GetGx() {
    return gX;
}


GLfloat Arena::GetGy() {
    return gY;
}


GLfloat Arena::GetWidth() {
    return gWidth;
}


GLfloat Arena::GetHeight() {
    return gHeight;
}


GLfloat Arena::GetThickness() {
    return gThickness;
}

Character * Arena::GetPlayer(){
    return gPlayer;
}


GLfloat Arena::GetPlayerGx() {
    return gPlayer->GetGx();
}


GLfloat Arena::GetPlayerGy() {
    return gPlayer->GetGy();
}


GLfloat Arena::GetPlayerGz() {
    return gPlayer->GetGz();
}


void Arena::MovePlayerInXZ(GLdouble timeDifference) {
    for (Obstacle* obstacle : gObstacles) {
        if(CharacterCollidesWithObstacle(gPlayer, obstacle, timeDifference) && !CharacterLandsInObstacle(gPlayer, obstacle, timeDifference)) {
            return;
        }
    }

    for (Opponent* opponent : gOpponents) {
        if (CharacterCollidesWithOpponent(gPlayer, opponent, timeDifference) && !CharacterLandsInOpponent(gPlayer, opponent, timeDifference)) {
            return;
        }
    }

    gPlayer->MoveInXZ(gX, gX + gWidth, -gThickness, 0, timeDifference);
}


void Arena::MovePlayerInY(GLdouble timeDifference) {
    for (Obstacle* obstacle : gObstacles) {
        if (CharacterLandsInObstacle(gPlayer, obstacle, timeDifference)) {
            return;
        }
    }

    for (Opponent* opponent : gOpponents) {
        if (CharacterLandsInOpponent(gPlayer, opponent, timeDifference)) {
            return;
        }
    }

    gPlayer->MoveInY(gY, gY + gHeight, timeDifference);
}

void Arena::RotatePlayer(bool clockwise, GLdouble timeDifference) {
    gPlayer->Rotate(clockwise, timeDifference);
}


bool Arena::CharacterCollidesWithObstacle(Character * character, Obstacle* obstacle, GLdouble timeDifference) {
    
    // Ajustes de offset
    GLfloat offsetXZ = 0.05f;
    GLfloat offsetY = 0.05f;

    // Preview da posição do jogador após o movimento
    GLfloat angleRad = character->GetXZAngle() * M_PI / 180.0f;
    GLfloat gXPreview = character->GetGx() + (character->GetXZSpeed() + offsetXZ) * timeDifference * character->GetMovementDirection() * cos(angleRad);
    GLfloat gYPreview = character->GetGy() + (character->GetYSpeed() + offsetY) * timeDifference * character->GetYDirection();

    // Hitbox do jogador
    GLfloat playerLeftX = gXPreview - character->GetHitboxRadius();
    GLfloat playerRightX = gXPreview + character->GetHitboxRadius();
    GLfloat playerBottomY = gYPreview - character->GetThighHeight() - character->GetShinHeight();
    GLfloat playerTopY = playerBottomY + character->GetHitboxHeight();

    // Hitbox do obstáculo
    GLfloat obstacleLeftX = obstacle->GetGx();
    GLfloat obstacleRightX = obstacle->GetGx() + obstacle->GetWidth();
    GLfloat obstacleTopY = obstacle->GetGy();
    GLfloat obstacleBottomY = obstacle->GetGy() - obstacle->GetHeight();

    // Verificar colisão em X e Y
    bool collidesInX = playerRightX >= obstacleLeftX && playerLeftX <= obstacleRightX;
    bool collidesInY = playerBottomY < obstacleTopY && playerTopY > obstacleBottomY;

    // Evitar "travar na quina" verificando se o jogador está subindo
    bool isMovingUp = character->GetYDirection() > 0;
    if (isMovingUp && playerBottomY >= obstacleTopY - offsetY) {
        return false;  // Permite que o jogador continue subindo
    }

    return collidesInX && collidesInY;
}


bool Arena::CharacterLandsInObstacle(Character * character, Obstacle* obstacle, GLdouble timeDifference) {
    
    // Ajustes de offset
    GLfloat offsetXZ = 0.05f;
    GLfloat offsetY = 0.05f;

    // Preview da posição do jogador após o movimento
    GLfloat angleRad = character->GetXZAngle() * M_PI / 180.0f;
    GLfloat gXPreview = character->GetGx() + (character->GetXZSpeed() + offsetXZ) * timeDifference * character->GetMovementDirection() * cos(angleRad);
    GLfloat gYPreview = character->GetGy() + (character->GetYSpeed() + offsetY) * timeDifference * character->GetYDirection();

    // Coordenadas do jogador
    GLfloat playerBottomY = gYPreview - character->GetThighHeight() - character->GetShinHeight();
    GLfloat playerTopY = playerBottomY + character->GetHitboxHeight();

    // Coordenadas do obstáculo
    GLfloat obstacleTopY = obstacle->GetGy();
    GLfloat obstacleLeftX = obstacle->GetGx();
    GLfloat obstacleRightX = obstacle->GetGx() + obstacle->GetWidth();

    GLfloat auxOffset = 0.0f; // Offset que ajuda na descida/subida do player qundo está na borda do obstaculo
    if (character->GetYDirection() < 0) auxOffset = 1.0f;
    else                              auxOffset = 0.0f;      

    bool playerIsWithinX = gXPreview >= obstacleLeftX - auxOffset && gXPreview <= obstacleRightX + auxOffset;
    bool playerIsAbove = playerTopY >= obstacleTopY && playerBottomY <= obstacleTopY && playerBottomY > obstacleTopY - 50 * offsetY;

    bool playerLandedInObstacle = playerIsWithinX && playerIsAbove;

    // Fixando o player encima do obstaculo para evitar bugs
    if (playerLandedInObstacle) {
        character->SetGy(obstacleTopY + character->GetThighHeight() + character->GetShinHeight());
    }

    return playerLandedInObstacle;
}


bool Arena::CharacterCollidesWithOpponent(Character * character, Opponent* opponent, GLdouble timeDifference) {
    // Ajustes de offset
    GLfloat offsetXZ = 0.05f;
    GLfloat offsetY = 0.05f;

    // Preview da posição do jogador após o movimento
    GLfloat playerAngleRad = character->GetXZAngle() * M_PI / 180.0f;
    GLfloat playerGXPreview = character->GetGx() + (character->GetXZSpeed() + offsetXZ) * timeDifference * character->GetMovementDirection() * cos(playerAngleRad);
    GLfloat playerGZPreview = character->GetGz() + (character->GetXZSpeed() + offsetXZ) * timeDifference  * character->GetMovementDirection() * sin(playerAngleRad);

    // Pontos da hitbox do jogador
    GLfloat playerLeftX = playerGXPreview - character->GetHitboxRadius();
    GLfloat playerRightX = playerGXPreview + character->GetHitboxRadius();
    GLfloat playerBottomY = character->GetGy() - character->GetThighHeight() - character->GetShinHeight();
    GLfloat playerTopY = playerBottomY + character->GetHitboxHeight();
    GLfloat playerFrontZ = playerGZPreview + character->GetHitboxRadius();
    GLfloat playerBackZ = playerGZPreview - character->GetHitboxRadius();

    // Pontos da hitbox do oponente
    GLfloat opponentLeftX = opponent->GetGx() - opponent->GetHitboxRadius();
    GLfloat opponentRightX = opponent->GetGx() + opponent->GetHitboxRadius();
    GLfloat opponentBottomY = opponent->GetGy() - opponent->GetThighHeight() - opponent->GetShinHeight();
    GLfloat opponentTopY = opponentBottomY + opponent->GetHitboxHeight();
    GLfloat opponentFrontZ = opponent->GetGz() + opponent->GetHitboxRadius();
    GLfloat opponentBackZ = opponent->GetGz() - opponent->GetHitboxRadius();

    // Verificar colisão em X, Y e Z
    bool collidesInX = playerRightX >= opponentLeftX && playerLeftX <= opponentRightX;
    bool collidesInY = playerTopY >= opponentBottomY && playerBottomY <= opponentTopY;
    bool collidesInZ = playerFrontZ >= opponentBackZ && playerBackZ <= opponentFrontZ;

    // Evitar "travar na quina" verificando se o jogador está subindo
    bool isMovingUp = character->GetYDirection() > 0;
    if (isMovingUp && playerBottomY >= opponentTopY - offsetY) {
        return false;  // Permite que o jogador continue subindo
    }

    return collidesInX && collidesInY && collidesInZ;
}

bool Arena::CharacterLandsInOpponent(Character * character, Opponent* opponent, GLdouble timeDifference) {

    // Ajustes de offset
    GLfloat offsetXZ = 0.05f;
    GLfloat offsetY = 0.05f;

    // Preview da posição do jogador após o movimento
    GLfloat playerAngleRad = character->GetXZAngle() * M_PI / 180.0f;
    GLfloat playerGXPreview = character->GetGx() + (character->GetXZSpeed() + offsetXZ) * timeDifference * character->GetMovementDirection() * cos(playerAngleRad);
    GLfloat playerGZPreview = character->GetGz() + (character->GetXZSpeed() + offsetXZ) * timeDifference  * character->GetMovementDirection() * sin(playerAngleRad);

    // Pontos da hitbox do jogador
    GLfloat playerBottomY = character->GetGy() - character->GetThighHeight() - character->GetShinHeight();
    GLfloat playerTopY = playerBottomY + character->GetHitboxHeight();

    // Pontos da hitbox do oponente
    GLfloat opponentLeftX = opponent->GetGx() - opponent->GetHitboxRadius();
    GLfloat opponentRightX = opponent->GetGx() + opponent->GetHitboxRadius();
    GLfloat opponentBottomY = opponent->GetGy() - opponent->GetThighHeight() - opponent->GetShinHeight();
    GLfloat opponentTopY = opponentBottomY + opponent->GetHitboxHeight();
    GLfloat opponentFrontZ = opponent->GetGz() + opponent->GetHitboxRadius();
    GLfloat opponentBackZ = opponent->GetGz() - opponent->GetHitboxRadius();

    GLfloat auxOffset = 0.0f; // Offset que ajuda na descida/subida do player qundo está na borda do obstaculo
    if (character->GetYDirection() < 0) auxOffset = 1.0f;
    else                              auxOffset = 0.0f;   

    bool playerIsWithinX = playerGXPreview >= opponentLeftX - auxOffset && playerGXPreview <= opponentRightX + auxOffset;
    bool playerIsWithinZ = playerGZPreview >= opponentBackZ - auxOffset && playerGZPreview <= opponentFrontZ + auxOffset;
    bool playerIsAbove = playerTopY >= opponentTopY && playerBottomY <= opponentTopY && playerBottomY > opponentTopY - 50 * offsetY;

    bool playerLandsInOpponent = playerIsWithinX && playerIsAbove && playerIsWithinZ;

    // Fixando o player encima do oponente para evitar bugs
    if (playerLandsInOpponent) {
        character->SetGy(opponentTopY + character->GetThighHeight() + character->GetShinHeight());
    }

    return playerLandsInOpponent;
}


void Arena::RotatePlayerArm(GLfloat x, GLfloat y, GLfloat windowWidth, GLfloat windowHeight, GLdouble timeDifference) {
    gPlayer->RotateArm(x, y, windowWidth, windowHeight, timeDifference);
}


void Arena::SetPlayerMovementDirection(GLint direction) {
    gPlayer->SetMovementDirection(direction);
}


void Arena::SetPlayerYDirection(GLint yDirection) {
    gPlayer->SetYDirection(yDirection);
}


GLfloat Arena::GetPlayerFrontThighAngle() {
    return gPlayer->GetFrontThighAngle();
}


GLfloat Arena::GetPlayerBackThighAngle() {
    return gPlayer->GetBackThighAngle();
}


GLfloat Arena::GetPlayerFrontShinAngle() {
    return gPlayer->GetFrontShinAngle();
}


GLfloat Arena::GetPlayerBackShinAngle() {
    return gPlayer->GetBackShinAngle();
}


void Arena::RotatePlayerFrontThigh(GLfloat angle, GLdouble timeDifference) {
    gPlayer->RotateFrontThigh(angle * gPlayer->GetXZSpeed(), timeDifference);
}


void Arena::RotatePlayerBackThigh(GLfloat angle, GLdouble timeDifference) {
    gPlayer->RotateBackThigh(angle * gPlayer->GetXZSpeed(), timeDifference);
}


void Arena::RotatePlayerFrontShin(GLfloat angle, GLdouble timeDifference) {
    gPlayer->RotateFrontShin(angle * gPlayer->GetXZSpeed(), timeDifference);
}


void Arena::RotatePlayerBackShin(GLfloat angle, GLdouble timeDifference) {
    gPlayer->RotateBackShin(angle * gPlayer->GetXZSpeed(), timeDifference);
}


void Arena::SetPlayerFrontShinAngle(GLfloat angle) {
    gPlayer->SetFrontShinAngle(angle);
}


void Arena::SetPlayerBackShinAngle(GLfloat angle) {
    gPlayer->SetBackShinAngle(angle);
}


Shot* Arena::PlayerShoot(GLfloat maxDist) {
    return gPlayer->Shoot(maxDist);
}


GLint Arena::GetPlayerMovementDirection() {
    return gPlayer->GetMovementDirection();
}


GLint Arena::GetPlayerYDirection() {
    return gPlayer->GetYDirection();
}


void Arena::PlayerJump() {
    gPlayer->Jump();
}


GLfloat Arena::GetPlayerMaxJumpHeight() {
    return gPlayer->GetMaxJumpHeight();
}


GLfloat Arena::GetPlayerJumpHeight() {
    return gPlayer->GetJumpHeight();
}


GLfloat Arena::GetPlayerThighHeight() {
    return gPlayer->GetThighHeight();
}


GLfloat Arena::GetPlayerShinHeight() {
    return gPlayer->GetShinHeight();
}


bool Arena::PlayerReachedMaximumJumpHeight() {
    return gPlayer->ReachedMaximumJumpHeight();
}


bool Arena::PlayerLanded(GLdouble timeDifference) {
    if (gPlayer->GetYDirection() == 1) return false; // If the player is still going up, it hasn't landed yet

    if (CharacterCollidesWithGround(gPlayer, 0, gPlayer->GetYSpeed())) {
        return true;
    }
    
    for (Obstacle* obstacle : gObstacles) {
        if (CharacterLandsInObstacle(gPlayer, obstacle, timeDifference)) {
            return true;
        }
    }

    for (Opponent* opponent : gOpponents) {
        if (CharacterLandsInOpponent(gPlayer, opponent, timeDifference)) {
            return true;
        }
    }

    return false;
}


bool Arena::OpponentLanded(Opponent* opponent, GLdouble timeDifference) {
    if (opponent->GetYDirection() == 1) return false; // If the opponent is still going up, it hasn't landed yet

    if (CharacterCollidesWithGround(opponent, 0, opponent->GetYSpeed())) {
        return true;
    }

    for (Obstacle* obstacle : gObstacles) {
        if (CharacterLandsInObstacle(opponent, obstacle, timeDifference)) {
            return true;
        }
    }

    if (OpponentLandsInPlayer(opponent, gPlayer, 0, opponent->GetYSpeed())) {
        return true;
    }

    for (Opponent* otherOpponent : gOpponents) {
        if (opponent != otherOpponent && CharacterCollidesWithOpponent(opponent, otherOpponent, timeDifference)) {
            return true;
        }
    }

    return false;
}


bool Arena::CharacterCollidesWithGround(Character * character, GLfloat dx, GLfloat dy) {
    GLfloat offset = 1.0f; // Offset to avoid collision detection problems
    return character->GetGy() - character->GetThighHeight() - character->GetShinHeight() <= gY + ((dy + offset) * (-character->GetYDirection()));
}


std::vector<Obstacle*> Arena::GetObstacles() {
    return gObstacles;
}


std::vector<Opponent*> Arena::GetOpponents() {
    return gOpponents;
}


void Arena::EraseOpponent(Opponent* opponent) {
    for (size_t i = 0; i < gOpponents.size(); ++i) {
        if (gOpponents[i] == opponent) {
            gOpponents.erase(gOpponents.begin() + i);
            return;
        }
    }
}


void Arena::MoveOpponentsInY(GLdouble timeDifference) {
    for (Opponent* opponent : gOpponents) {
        bool opponentCollided = false;
        
        for (Obstacle* obstacle : gObstacles) {
            if (CharacterCollidesWithObstacle(opponent, obstacle, timeDifference)) {
                opponentCollided = true;
            }
        }

        if (opponentCollided) continue;

        // Opponent landed in the player //FIXME:
        if (OpponentLandsInPlayer(opponent, gPlayer, 0, opponent->GetYSpeed())) {
            continue;
        }

        opponent->MoveInY(gY, gY + gHeight, timeDifference);
    }
}


void Arena::MoveOpponentsInXZ(GLdouble timeDifference) {
    for (Opponent* opponent : gOpponents) {
        if (!OpponentLanded(opponent, timeDifference)) { continue; }

        bool directionChanged = false;

        // When opponent is landing on an obstacle
        for (Obstacle* obstacle : gObstacles) {
            if (CharacterLandsInObstacle(opponent, obstacle, timeDifference)) {
                GLfloat obstacleLeftX = obstacle->GetGx();
                GLfloat obstacleRightX = obstacle->GetGx() + obstacle->GetWidth();

                // If the opponent reaches the left or right side of the obstacle, change direction
                if ((opponent->GetGx() + (opponent->GetHitboxRadius() / 2) >= obstacleRightX - 2.0f && opponent->GetMovementDirection() == 1) || // 1.0f offset
                    (opponent->GetGx() - (opponent->GetHitboxRadius() / 2) <= obstacleLeftX + 2.0f && opponent->GetMovementDirection() == -1)) { // 1.0f offset
                    opponent->SetMovementDirection(-opponent->GetMovementDirection());
                    directionChanged = true;
                }
                break;
            }
        }

        // Check player collision
        if (CharacterCollidesWithOpponent(gPlayer, opponent, timeDifference)) {
            if (!CharacterLandsInOpponent(gPlayer, opponent, timeDifference)) {
                continue; // Do not move
            }
        }

        // If the opponent is not on an obstacle, check for collisions on the ground
        if (!directionChanged && CharacterCollidesWithGround(opponent, 0, opponent->GetYSpeed())) {
            for (Obstacle* obstacle : gObstacles) {
                if (CharacterCollidesWithObstacle(opponent, obstacle, timeDifference)) {
                    opponent->SetMovementDirection(-opponent->GetMovementDirection());
                    directionChanged = true;
                    break;
                }
            }
        }

        if (!directionChanged) {
            // Check collision with arena borders
            if ((opponent->GetGx() + opponent->GetHitboxRadius() / 2 >= gX + gWidth -1.0f && opponent->GetMovementDirection() == 1) ||
                (opponent->GetGx() - opponent->GetHitboxRadius() / 2 <= gX + 1.0f && opponent->GetMovementDirection() == -1)) {
                opponent->SetMovementDirection(-opponent->GetMovementDirection());
                directionChanged = true;
            }
        }

        if (!directionChanged) {
            // If opponent collides with other opponent, both change direction
            for (Opponent* otherOpponent : gOpponents) {
                if (opponent != otherOpponent && CharacterCollidesWithOpponent(opponent, otherOpponent, timeDifference)) {
                    opponent->SetMovementDirection(-opponent->GetMovementDirection());
                    otherOpponent->SetMovementDirection(-otherOpponent->GetMovementDirection());
                    directionChanged = true;
                    break;
                }
            }
        }

        if (!directionChanged) {
            opponent->MoveInXZ(gX, gX + gWidth, -gThickness, 0, timeDifference);
        }
    }
}

bool Arena::OpponentLandsInPlayer(Opponent* opponent, Player* player, GLfloat dx, GLfloat dy) {
    GLfloat offsetX = 1.0f; // Offset to avoid collision detection problems
    GLfloat offsetY = 1.0f; // Offset to avoid collision detection problems

    GLfloat x = opponent->GetGx() + ((dx + offsetX) * opponent->GetMovementDirection());
    GLfloat y = opponent->GetGy() + ((dy + offsetY) * opponent->GetYDirection());

    GLfloat opponentLeftX = x - opponent->GetHitboxRadius() / 2;
    GLfloat opponentRightX = x + opponent->GetHitboxRadius() / 2;
    GLfloat opponentTopY = y - opponent->GetThighHeight() - opponent->GetShinHeight() + opponent->GetHitboxHeight();
    GLfloat opponentBottomY = y - opponent->GetThighHeight() - opponent->GetShinHeight();

    GLfloat playerLeftX = player->GetGx() - player->GetHitboxRadius() / 2;
    GLfloat playerRightX = player->GetGx() + player->GetHitboxRadius() / 2;
    GLfloat playerTopY = player->GetGy() - player->GetThighHeight() - player->GetShinHeight() + player->GetHitboxHeight();

    return opponentTopY >= playerTopY && opponentBottomY <= playerTopY + offsetY &&
           opponentRightX >= playerLeftX && opponentLeftX <= playerRightX;
}

bool Arena::ObstaclesCollidesWithShot(Shot* shot) {
    for (Obstacle* obstacle : gObstacles) {
        if (ObstacleCollidesWithShot(obstacle, shot)) {
            return true;
        }
    }

    return false;
}


bool Arena::ObstacleCollidesWithShot(Obstacle* obstacle, Shot* shot) {
    // Don't need to check the z axis

    GLfloat shotX = shot->GetGx();
    GLfloat shotY = shot->GetGy();
    GLfloat shotRadius = shot->GetRadius();

    GLfloat closestX = std::max(obstacle->GetGx(), std::min(shotX, obstacle->GetGx() + obstacle->GetWidth()));
    GLfloat closestY = std::max(obstacle->GetGy() - obstacle->GetHeight(), std::min(shotY, obstacle->GetGy()));

    GLfloat distanceX = shotX - closestX;
    GLfloat distanceY = shotY - closestY;
    GLfloat distanceSquared = distanceX * distanceX + distanceY * distanceY;

    return distanceSquared <= shotRadius * shotRadius;
}


bool Arena::OpponentsCollidesWithShot(Shot* shot) {
    for (Opponent* opponent : gOpponents) {
        if (CharacterCollidesWithShot(opponent, shot)) {
            EraseOpponent(opponent);
            return true;
        }
    }

    return false;
}


void Arena::MoveOpponentsArms(GLdouble timeDifference) {
    // Point opponents arms to the player

    for (Opponent* opponent : gOpponents) {
        GLfloat playerX = gPlayer->GetGx();
        GLfloat playerY = gPlayer->GetGy();
        GLfloat playerZ = gPlayer->GetGz();

        GLfloat opponentX = opponent->GetGx();
        GLfloat opponentY = opponent->GetGy();
        GLfloat opponentZ = opponent->GetGz();

        GLfloat xResVector = playerX - opponentX;
        GLfloat yResVector = playerY - opponentY;
        GLfloat zResVector = playerZ - opponentZ;

        GLfloat XYmaxAngle = -45.0f;
        GLfloat XYminAngle = -135.0f;
        GLfloat XYangle = 0.0f;

        // Special case: player and opponent at the same Y level
        if (fabs(yResVector) < 0.001f) { // Tolerance for float equality
            XYangle = -90.0f;
        } else {
            XYangle = atan2(yResVector, xResVector) * 180.0f / M_PI;

            // Adjust the angle to the arm limits
            XYangle = XYangle - 90.0f;
            if (XYangle > 0) XYangle *= -1;

            if      (XYangle > XYmaxAngle) XYangle = XYmaxAngle;
            else if (XYangle < XYminAngle) XYangle = XYminAngle;
        }

        GLfloat XZmaxAngle = 45.0f;
        GLfloat XZminAngle = -45.0f;
        GLfloat XZangle = 0.0f;

        // Special case: player and opponent at the same Z level
        if (fabs(zResVector) < 0.001f) { // Tolerance for float equality
            XZangle = 0.0f;
        } else {
            XZangle = atan2(zResVector, xResVector) * 180.0f / M_PI;

            if      (XZangle > XZmaxAngle) XZangle = XZmaxAngle;
            else if (XZangle < XZminAngle) XZangle = XZminAngle;
        }

        opponent->RotateArmToTargetAngle(timeDifference, XZangle, XYangle);
    }
}


void Arena::UpdateOpponentsShots(std::vector<Shot*>& opponentsShots, GLfloat maxDist, GLdouble timeDifference) {
    for (Opponent* opponent : gOpponents) {
        if (rand() % 2 == 0) {
            opponentsShots.push_back(opponent->Shoot(maxDist));
        }
    }
}


bool Arena::CharacterCollidesWithShot(Character * character, Shot* shot) {
    GLfloat shotX = shot->GetGx();
    GLfloat shotY = shot->GetGy();
    GLfloat shotZ = shot->GetGz();
    GLfloat shotRadius = shot->GetRadius();

    GLfloat playerX = character->GetGx();
    GLfloat playerY = character->GetGy();
    GLfloat playerZ = character->GetGz();
    GLfloat hitboxRadius = character->GetHitboxRadius();
    GLfloat hitboxHeight = character->GetHitboxHeight();

    GLfloat playerTopY = playerY - character->GetThighHeight() - character->GetShinHeight() + hitboxHeight;
    GLfloat playerBottomY = playerY - character->GetThighHeight() - character->GetShinHeight();

    // Distância do tiro ao eixo central do cilindro no plano XZ
    GLfloat distanceX = shotX - playerX;
    GLfloat distanceZ = shotZ - playerZ;
    GLfloat distanceXZ = sqrt(distanceX * distanceX + distanceZ * distanceZ);

    // Verifica se o tiro está dentro da altura do cilindro (considerando o raio do tiro)
    bool insideHeight = (shotY + shotRadius >= playerBottomY) && (shotY - shotRadius <= playerTopY);

    // Verifica se o tiro está dentro do raio do cilindro no plano XZ
    bool insideRadius = (distanceXZ <= hitboxRadius + shotRadius);

    return insideHeight && insideRadius;
}

bool Arena::PlayerHitsHead() {
    if (PlayerHitsHeadRoof(gPlayer, 0, gPlayer->GetYSpeed())) { 
        printf("Player hits head\n");
        return true;
    }

    for (Obstacle* obstacle : gObstacles) {
        if (PlayerHitsHeadObstacle(gPlayer, obstacle, 0, gPlayer->GetYSpeed())) {
            return true;
        }
    }

    for (Opponent* opponent : gOpponents) {
        if (PlayerHitsHeadOpponent(gPlayer, opponent, 0, gPlayer->GetYSpeed())) {
            return true;
        }
    }

    return false;
}


bool Arena::PlayerHitsHeadRoof(Player* player, GLfloat dx, GLfloat dy) {
    GLfloat offset = 1.0f;
    return player->GetGy() - player->GetThighHeight() - player->GetShinHeight() + player->GetHitboxHeight() >= gY + gHeight - dy - offset;
}


bool Arena::PlayerHitsHeadObstacle(Player* player, Obstacle* obstacle, GLfloat dx, GLfloat dy) {
    GLfloat offsetX = 1.0f;
    GLfloat offsetY = 1.0f;

    GLfloat x = player->GetGx() + ((dx + offsetX) * player->GetMovementDirection());
    GLfloat y = player->GetGy() + ((dy + offsetY) * player->GetYDirection());

    GLfloat playerLeftX = x - player->GetHitboxRadius() / 2;
    GLfloat playerRightX = x + player->GetHitboxRadius() / 2;
    GLfloat playerTopY = y - player->GetThighHeight() - player->GetShinHeight() + player->GetHitboxHeight();
    GLfloat playerBottomY = y - player->GetThighHeight() - player->GetShinHeight();

    GLfloat obstacleLeftX = obstacle->GetGx();
    GLfloat obstacleRightX = obstacle->GetGx() + obstacle->GetWidth();
    GLfloat obstacleBottomY = obstacle->GetGy() - obstacle->GetHeight();

    return playerTopY >= obstacleBottomY && playerBottomY <= obstacleBottomY + offsetY &&
           playerRightX >= obstacleLeftX && playerLeftX <= obstacleRightX;
}


bool Arena::PlayerHitsHeadOpponent(Player* player, Opponent* opponent, GLfloat dx, GLfloat dy) {
    GLfloat offsetX = 1.0f;
    GLfloat offsetY = 1.0f;

    GLfloat x = player->GetGx() + ((dx + offsetX) * player->GetMovementDirection());
    GLfloat y = player->GetGy() + ((dy + offsetY) * player->GetYDirection());

    GLfloat playerLeftX = x - player->GetHitboxRadius() / 2;
    GLfloat playerRightX = x + player->GetHitboxRadius() / 2;
    GLfloat playerTopY = y - player->GetThighHeight() - player->GetShinHeight() + player->GetHitboxHeight();
    GLfloat playerBottomY = y - player->GetThighHeight() - player->GetShinHeight();

    GLfloat opponentLeftX = opponent->GetGx() - opponent->GetHitboxRadius() / 2;
    GLfloat opponentRightX = opponent->GetGx() + opponent->GetHitboxRadius() / 2;
    GLfloat opponentBottomY = opponent->GetGy() - opponent->GetThighHeight() - opponent->GetShinHeight();

    return playerTopY >= opponentBottomY && playerBottomY <= opponentBottomY + offsetY &&
           playerRightX >= opponentLeftX && playerLeftX <= opponentRightX;
}


bool Arena::PlayerWon() {
    GLfloat offset = 10.0f;
    GLfloat endArena = gX + gWidth - offset;
    GLfloat gXPreview = gPlayer->GetGx() + gPlayer->GetHitboxRadius(); 
    return gXPreview >= endArena;
}


void Arena::Delete() {
    delete gPlayer;

    for (Obstacle* obstacle : gObstacles) {
        delete obstacle;
    }

    for (Opponent* opponent : gOpponents) {
        delete opponent;
    }
}


void Arena::AnimatePlayerLegs(GLdouble timeDifference) {
    gPlayer->AnimateLegs(timeDifference);
}


void Arena::AnimateOpponentsLegs(GLdouble timeDifference) {
    for (Opponent* opponent : gOpponents) {
        opponent->AnimateLegs(timeDifference);
    }
}


GLfloat Arena::CalculatePlayerHeadYPosition() {
    return gPlayer->GetGy() - gPlayer->GetThighHeight() - gPlayer->GetShinHeight() + gPlayer->GetHitboxHeight();
}


GLfloat Arena::GetPlayerXZAngle() {
    return gPlayer->GetXZAngle();
}


GLfloat Arena::GetPlayerXZArmAngle() {
    return gPlayer->GetXZArmAngle();
}


GLfloat Arena::GetPlayerXYArmAngle() {
    return gPlayer->GetXYArmAngle();
}


void Arena::CalculatePlayerArmTopPos(GLfloat* playerArmTopPos) {
    gPlayer->CalculateArmTopPos(playerArmTopPos);
}


void Arena::CalculatePlayerArmLookAt(GLfloat* playerArmLookAt) {
    gPlayer->CalculateArmLookAt(playerArmLookAt);
}

void Arena::DrawHitboxes(){
    gPlayer->DrawHitbox();

    for (Opponent* opponent : gOpponents) {
        opponent->DrawHitbox();
    }
}
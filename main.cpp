/*****************************************************************************************/
/******************************* INCLUDES AND DEFINES ************************************/
/*****************************************************************************************/

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include "tinyxml2.h"
#include <string>
#include "arena.h"
#include "shot.h"
#include "imageloader.h"


using namespace tinyxml2;



/*****************************************************************************************/
/*********************************** GLOBAL VARIABLES ************************************/
/*****************************************************************************************/

int keyStatus[256];

// Window dimensions
int Width = 500;
int Height = 500;

// Viewing dimensions
GLfloat viewingWidth = 0;
GLfloat viewingHeight = 0;
GLfloat xPositionArena = 0;
GLfloat yPositionArena = 0;
GLfloat arenaWidth = 0;
GLfloat arenaHeight = 0;
GLfloat viewPortLeft = 0;
GLfloat viewPortRight = 0;
GLfloat viewPortBottom = 0;
GLfloat viewPortTop = 0;

// Components of the virtual world
Arena* arena = NULL;
std::vector<Shot*> playerShots;
std::vector<Shot*> opponentsShots;

// Camera
double camXYAngle = 0;
double camXZAngle = 0;
int lastX = 0;
int lastY = 0;
int thirdCameraZoom = 10;
float cameraHeightOffset = 5.0f;

// Flags and aux variables
char* svgFilePath = NULL;
float positionTolerance = 0.5f;
float mouseY = 0.0f;
float mouseX = 0.0f;
GLfloat timeAccumulator = 0.0f;
void* font = GLUT_BITMAP_9_BY_15;
int gameOver = 0;
int playerWon = 0;
int toggleCam = 3;

// Feature flags
int simulateSlowProcessingUbuntu = 0;
int simulateSlowProcessingWindows = 0;
int opponentMoves = 0;
int opponentShoots = 0;
int moveThirdCamera = 0;
int visibleHitboxes = 0;
int nightMode = 0;

// Textures
GLuint arenaGroundTexture;
GLuint arenaWallTexture;
GLuint arenaRoofTexture;



/*****************************************************************************************/
/************************************ AUX FUNCTIONS **************************************/
/*****************************************************************************************/
GLuint LoadTextureRAW(const char* filename) {
    GLuint texture;
    Image* image = loadBMP(filename);

    glGenTextures(1, &texture); // Cria o container de textura
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE);
	// glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
                             0,                            //0 for now
                             GL_RGB,                       //Format OpenGL uses for image
                             image->width, image->height,  //Width and height pixels
                             0,                            //The border of the image
                             GL_RGB, //GL_RGB, because pixels are stored in RGB format
                             GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
                                               //as unsigned numbers
                             image->pixels);               //The actual pixel data
    delete image;

    return texture;
}


void Normalize(GLfloat* v) {
    GLfloat length = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    if (length > 1e-6) { // Evita divisão por zero
        v[0] /= length;
        v[1] /= length;
        v[2] /= length;
    }
}


void CrossProduct(GLfloat* a, GLfloat* b, GLfloat* result) {
    result[0] = a[1] * b[2] - a[2] * b[1];
    result[1] = a[2] * b[0] - a[0] * b[2];
    result[2] = a[0] * b[1] - a[1] * b[0];
}


void RasterChars(GLfloat x, GLfloat y, GLfloat z, const char* text, double r, double g, double b) {
    glPushAttrib(GL_ENABLE_BIT);
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);

        glColor3f(r, g, b);
        glRasterPos3f(x, y, z);
        const char* tmpStr;
        tmpStr = text;

        while (*tmpStr) {
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *tmpStr);
            tmpStr++;
        }
    glPopAttrib();
}


void PrintText(GLfloat x, GLfloat y, const char* text, double r, double g, double b) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
        glLoadIdentity ();
        glOrtho(0, 1, 0, 1, -1, 1);
        RasterChars(x, y, 0, text, r, g, b);    
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}


void DrawAxes() {
    GLfloat color_r[] = { 1.0, 0.0, 0.0, 1.0 };
    GLfloat color_g[] = { 0.0, 1.0, 0.0, 1.0 };
    GLfloat color_b[] = { 0.0, 0.0, 1.0, 1.0 };

    glPushAttrib(GL_ENABLE_BIT);
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
 
        // X axis
        glPushMatrix();
            glColor3fv(color_r);
            glScalef(5, 0.3, 0.3);
            glTranslatef(0.5, 0, 0); // Put in one end
            glutSolidCube(1.0);
        glPopMatrix();

        // Y axis
        glPushMatrix();
            glColor3fv(color_g);
            glRotatef(90, 0, 0, 1);
            glScalef(5, 0.3, 0.3);
            glTranslatef(0.5, 0, 0); // Put in one end
            glutSolidCube(1.0);
        glPopMatrix();

        // Z axis
        glPushMatrix();
            glColor3fv(color_b);
            glRotatef(-90, 0, 1, 0);
            glScalef(5, 0.3, 0.3);
            glTranslatef(0.5, 0, 0); // Put in one end
            glutSolidCube(1.0);
        glPopMatrix();
    glPopAttrib();
}


bool loadViewportSizeFromSvg(const char* svg_file_path) {
	XMLDocument doc;
    if (doc.LoadFile(svg_file_path) != XML_SUCCESS) {
        printf("Error loading SVG file: %s\n", svg_file_path);
        return false;
    }

    XMLElement* root = doc.FirstChildElement("svg");
    if (!root) {
        printf("<svg> element not found in file\n");
        return false;
    }

    for (XMLElement* elem = root->FirstChildElement("rect"); elem; elem = elem->NextSiblingElement("rect")) {
        const char* fill = elem->Attribute("fill");
        if (fill && std::string(fill) == "blue") {
            if (elem->Attribute("width") && elem->Attribute("height")) {
				arenaWidth = elem->FloatAttribute("width");
				arenaHeight = elem->FloatAttribute("height");
				xPositionArena = elem->FloatAttribute("x");
				yPositionArena = elem->FloatAttribute("y");
			} else {
				printf("Width or height attribute missing in blue rect.\n");
				return false;
			}

			viewingWidth = arenaWidth;
			viewingHeight = arenaHeight;

			// Make the viewing area a square with the smallest dimension
			if (viewingWidth > viewingHeight) {
				viewingWidth = viewingHeight;
			} else {
				viewingHeight = viewingWidth;
			}

            return true;
        }
    }

    printf("Blue rect not found in SVG file: %s\n", svg_file_path);
    return false;
}


void renderScene(void) {	
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	if (gameOver) {
		PrintText(0.5, 0.8, "Game Over", 1, 0, 0);		
	}
	
	if (playerWon) {
		PrintText(0.5, 0.8, "Player Won", 1, 1, 1);
	}

	if (toggleCam == 1){
        PrintText(0.1, 0.1, "First person camera", 0, 1, 0);
		glRotatef(-arena->GetPlayerXZAngle(), 0, 1, 0);
		glRotatef(90, 0, 1, 0); // Rotate to point to x positive
		glTranslatef(-arena->GetPlayerGx(), -arena->CalculatePlayerHeadYPosition(), -arena->GetPlayerGz());
 
    } else if (toggleCam == 2){
        PrintText(0.1, 0.1, "Gun sight camera", 0, 1, 0);

		GLfloat playerArmTopPos[3];
		arena->CalculatePlayerArmTopPos(playerArmTopPos);

		GLfloat playerArmLookAt[3];
		arena->CalculatePlayerArmLookAt(playerArmLookAt); // Já é um vetor de direção normalizado

		// 1. Definir um vetor worldUp genérico
		GLfloat worldUp[3] = {0.0f, 1.0f, 0.0f};

		// 2. Calcular o vetor "right" (perpendicular entre worldUp e playerArmLookAt)
		GLfloat right[3];
		CrossProduct(worldUp, playerArmLookAt, right);
		Normalize(right);

		// Se "right" for nulo (caso raro, quando o braço está perfeitamente alinhado com o eixo Y)
		if (fabs(right[0]) < 1e-6 && fabs(right[1]) < 1e-6 && fabs(right[2]) < 1e-6) {
			worldUp[0] = 1.0f; worldUp[1] = 0.0f; worldUp[2] = 0.0f; // Ajuste alternativo
			CrossProduct(worldUp, playerArmLookAt, right);
			Normalize(right);
		}

		// 3. Calcular "up" (perpendicular a "right" e "playerArmLookAt")
		GLfloat upVector[3];
		CrossProduct(playerArmLookAt, right, upVector);
		Normalize(upVector);

		// 4. Configurar a câmera
		gluLookAt(playerArmTopPos[0], playerArmTopPos[1], playerArmTopPos[2],
				playerArmTopPos[0] + playerArmLookAt[0], 
				playerArmTopPos[1] + playerArmLookAt[1], 
				playerArmTopPos[2] + playerArmLookAt[2], 
				upVector[0], upVector[1], upVector[2]);


    } else if (toggleCam == 3){
        PrintText(0.1, 0.1, "Third person camera", 0, 1, 0);
		
		// Calculate the angle to point to player
		float deltaY = cameraHeightOffset;
		float deltaX = thirdCameraZoom;
		float angle = atan(deltaY/deltaX) * 180 / M_PI;

		// Move away from the player
		if (!moveThirdCamera) glTranslatef(0, -cameraHeightOffset, -thirdCameraZoom);
		else                  glTranslatef(0, 0, -thirdCameraZoom);

		// Mouse rotation
		if (moveThirdCamera) {
			glRotatef(camXZAngle, 1, 0, 0);
			glRotatef(camXYAngle, 0, 1, 0);
		}

		// Rotate camera to point to x positive
		glRotatef(90, 0, 1, 0);

		// Rotate angle in y negative
		if (!moveThirdCamera) glRotatef(angle, 0, 0, 1);

		// We want that this next translation do not interfere with the camera rotation
		glTranslatef(-arena->GetPlayerGx(), -arena->GetPlayerGy(), -arena->GetPlayerGz());
    }

	// A PARTIR DAQUI, ESTAMOS NO SISTEMA DE COORDENADAS DO MUNDO
	
	GLfloat light_position[4];
	GLfloat light_direction[3];

	if (nightMode) {
		GLfloat playerArmTopPos[3];
		arena->CalculatePlayerArmTopPos(playerArmTopPos);
		light_position[0] = playerArmTopPos[0];
		light_position[1] = playerArmTopPos[1];
		light_position[2] = playerArmTopPos[2];
		light_position[3] = 1.0;

		GLfloat playerArmLookAt[3];
		arena->CalculatePlayerArmLookAt(playerArmLookAt);
		light_direction[0] = playerArmLookAt[0];
		light_direction[1] = playerArmLookAt[1];
		light_direction[2] = playerArmLookAt[2];
	
	} else {
		light_position[0] = arena->GetGx() + arena->GetWidth() / 2;
		light_position[1] = arena->GetGy() + arena->GetHeight() - 20;
		light_position[2] = -arena->GetThickness() / 2;
		light_position[3] = 1.0;
	}

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	if (nightMode) {
		glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_direction);
		glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 30.0); // Ângulo do cone de luz (0° a 90°)
		glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 10.0); // Intensidade dentro do cone (0 = uniforme, 128 = forte no centro)
	} else {
		glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 180.0); // Ângulo do cone de luz (0° a 90°)
	}

	
	glPushMatrix();
		glTranslatef(arena->GetPlayerGx(), arena->GetPlayerGy(), arena->GetPlayerGz());
		DrawAxes();
	glPopMatrix();

	arena->Draw();

	if (visibleHitboxes){
		arena->DrawHitboxes();
	}

	for (Shot* shot : playerShots) {
		if (shot) shot->Draw();
	}

	for (Shot* shot : opponentsShots) {
		if (shot) shot->Draw();
	}

	// Draw on the frame buffer
	glutSwapBuffers(); // Function suitable for double buffer window
}


void keyPress(unsigned char key, int x, int y) {
	switch (key) {
		case '1':
			toggleCam = 1;
			break;
		case '2':
			toggleCam = 2;
			break;
		case '3':
			toggleCam = 3;
			break;
		case '4':
			simulateSlowProcessingUbuntu = !simulateSlowProcessingUbuntu;
			break;
		case '5':
			simulateSlowProcessingWindows = !simulateSlowProcessingWindows;
			break;
		case '6':
			opponentMoves = !opponentMoves;
			break;
		case '7':
			opponentShoots = !opponentShoots;
			break;
		case '8':
			gameOver = 1;
			break;
		case 'a':
		case 'A':
			keyStatus[(int)('a')] = 1;
			break;
		case 'd':
		case 'D':
			keyStatus[(int)('d')] = 1;
			break;
		case 'w':
		case 'W':
			keyStatus[(int)('w')] = 1;
			break;
		case 's':
		case 'S':
			keyStatus[(int)('s')] = 1;
			break;
		case 'n':
		case 'N':
		    nightMode = !nightMode;
			break;
		case 'f':
		case 'F':
			keyStatus[(int)('f')] = 1;
			break;
		case 'r':
		case 'R':
			keyStatus[(int)('r')] = 1;
			break;
		case 'g':
		case 'G':
			keyStatus[(int)('g')] = 1;
			break;
		case 't':
		case 'T':
			keyStatus[(int)('t')] = 1;
			break;
		case 'h':
		case 'H':
			keyStatus[(int)('h')] = 1;
			break;
		case 'c':
		case 'C':
			visibleHitboxes = !visibleHitboxes;
			break;
		case 'x':
		case 'X':
			moveThirdCamera = !moveThirdCamera;
			break;
		case 'y':
		case 'Y':
			keyStatus[(int)('y')] = 1;
			break;
		case ' ':
			keyStatus[(int)(' ')] = 1;
			break;
		case '+':
			// if (thirdCameraZoom > 5 && moveThirdCamera) thirdCameraZoom--;
			thirdCameraZoom--;
			break;
		case '-':
			// if (thirdCameraZoom < 10 && moveThirdCamera) thirdCameraZoom++;
			thirdCameraZoom++;
			break;
		case 27:
			exit(0);
	}

	glutPostRedisplay();
}


void keyUp(unsigned char key, int x, int y) {
	keyStatus[(int)(key)] = 0;
	glutPostRedisplay();
}


void ResetKeyStatus() {
	// Initialize keyStatus
	int i;
	for(i = 0; i < 256; i++) keyStatus[i] = 0;
}


void init(int windowSize, char* svg_file_path) {
    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);
	glEnable(GL_TEXTURE_2D);
	glDepthFunc(GL_LEQUAL);
    glEnable(GL_LIGHT0);

    glViewport(0, 0, (GLsizei) windowSize, (GLsizei) windowSize);

	// Defining camera parameters
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90, (GLfloat) windowSize / (GLfloat) windowSize, 1, 150);


	glMatrixMode(GL_MODELVIEW);
	
	if (!loadViewportSizeFromSvg(svgFilePath)) {
		exit(1);
	}

	arenaGroundTexture = LoadTextureRAW("textures/ground.bmp");
	arenaWallTexture = LoadTextureRAW("textures/wall.bmp");
	arenaRoofTexture = LoadTextureRAW("textures/roof.bmp");

	arena = new Arena(svgFilePath, arenaGroundTexture, arenaWallTexture, arenaRoofTexture);

	ResetKeyStatus();
}


void passiveMotion(int x, int y) {
	// Invert the y position
	mouseY = Height - y;
	mouseX = x;

	if (moveThirdCamera) {
		camXYAngle += x - lastX;
		camXZAngle += y - lastY;

		camXYAngle = (int) camXYAngle % 360;
		camXZAngle = (int) camXZAngle % 360;
		if (camXZAngle > 60) camXZAngle = 60;
		if (camXZAngle < -60) camXZAngle = -60;

		lastX = x;
		lastY = y;
	}    
}


void mouseMotion(int x, int y) {
	// Invert the y position
	mouseY = Height - y;
	mouseX = x;
}


void mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && !gameOver && !playerWon) {
        playerShots.push_back(arena->PlayerShoot(viewingWidth));
    }
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN && !gameOver && !playerWon) {
		toggleCam = 2;
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP && !gameOver && !playerWon) {
		// toggleCam = 1;
		toggleCam = 3;
	}
}


void ResetGame() {
    gameOver = 0;
    playerWon = 0;

    for (Shot* shot : playerShots) {
        delete shot;
    }
    playerShots.clear();

    for (Shot* shot : opponentsShots) {
        delete shot;
    }
    opponentsShots.clear();

    arena->Delete();

	// Reset global variables
	viewingWidth = 0;
	viewingHeight = 0;
	xPositionArena = 0;
	yPositionArena = 0;
	arenaWidth = 0;
	arenaHeight = 0;
	viewPortLeft = 0;
	viewPortRight = 0;
	viewPortBottom = 0;
	viewPortTop = 0;
	positionTolerance = 0.5f;
	mouseY = 0.0f;
	timeAccumulator = 0.0f;
	font = GLUT_BITMAP_9_BY_15;
	gameOver = 0;
	playerWon = 0;

	simulateSlowProcessingUbuntu = 0;
	simulateSlowProcessingWindows = 0;
	opponentMoves = 0;
	opponentShoots = 0;

	loadViewportSizeFromSvg(svgFilePath);
    
	arena = new Arena(svgFilePath, arenaGroundTexture, arenaWallTexture, arenaRoofTexture);
}


void idle(void) {
	if (simulateSlowProcessingUbuntu) for (int i = 0; i < 9000000; i++);
	if (simulateSlowProcessingWindows) for (int i = 0; i < 90000000; i++);

	static GLdouble previousTime = glutGet(GLUT_ELAPSED_TIME);
	GLdouble currentTime, timeDifference;
	currentTime = glutGet(GLUT_ELAPSED_TIME);    // Get the time that has passed since the start of the application
	timeDifference = currentTime - previousTime; // Calculates the elapsed time since the last frame
	previousTime = currentTime;                  // Update the time of the last frame that occurred

	// Avoids the program to crash when the difference is too high
	if (timeDifference <= 0.0f || timeDifference > 200.0f) timeDifference = 1.0f;
	timeAccumulator += timeDifference;
	
	// Check for reset game call
	if (keyStatus[(int)('r')] && (gameOver || playerWon)) {
		ResetGame();
	}

	// Check for end of the game
	if (gameOver || playerWon) return;

	// Player movement forwards and backwards
	if (keyStatus[(int)('w')]) {
		arena->SetPlayerMovementDirection(1);
		arena->MovePlayerInXZ(timeDifference);
	}
	if (keyStatus[(int)('s')]) {
		arena->SetPlayerMovementDirection(-1);
		arena->MovePlayerInXZ(timeDifference);
	}
	
	// Player rotational movement
	if (keyStatus[(int)('a')]) {
		arena->RotatePlayer(true, timeDifference);
	}
	if (keyStatus[(int)('d')]) {
		arena->RotatePlayer(false, timeDifference);
	}

	// Player jump
	if (keyStatus[(int)(' ')] && arena->PlayerLanded(timeDifference)) {
		arena->PlayerJump();
	}

	// Player arm movement
	arena->RotatePlayerArm(mouseX, mouseY, Width, Height, timeDifference);
	
	// Applying 'gravity' to player and opponents
	arena->MovePlayerInY(timeDifference);
	arena->MoveOpponentsInY(timeDifference);
	if (!keyStatus[(int)(' ')] || arena->PlayerReachedMaximumJumpHeight() || arena->PlayerHitsHead()) {
		arena->SetPlayerYDirection(-1);
	}

	// Moving opponents	randomly
	if (opponentMoves) arena->MoveOpponentsInXZ(timeDifference);
	arena->MoveOpponentsArms(timeDifference);

	// Firing opponents shots
	if (timeAccumulator >= 1000.0f) {
		if (opponentShoots) arena->UpdateOpponentsShots(opponentsShots, arenaWidth, timeDifference);
		timeAccumulator = 0.0f;
	}
	
	// Updating player shots
	for (size_t i = 0; i < playerShots.size(); ++i) {
        Shot* shot = playerShots[i];
        if (shot) {
            shot->Move(timeDifference);

			bool shotDeleted = false;

			if (arena->ObstaclesCollidesWithShot(shot)) {
				delete shot;
				playerShots.erase(playerShots.begin() + i);
				i--;
				shotDeleted = true;
			}

			if (shotDeleted) continue;

			if (arena->OpponentsCollidesWithShot(shot)) {
				delete shot;
				playerShots.erase(playerShots.begin() + i);
				i--;
				shotDeleted = true;
			}

			if (shotDeleted) continue;

            if (!shot->Valid()) {
                delete shot;
                playerShots.erase(playerShots.begin() + i);
                i--;
            }
        }
    }

	// Updating opponents shots
	for (size_t i = 0; i < opponentsShots.size(); ++i) {
		Shot* shot = opponentsShots[i];

		if (shot) {
			shot->Move(timeDifference);

			bool shotDeleted = false;

			if (arena->ObstaclesCollidesWithShot(shot)) {
				delete shot;
				opponentsShots.erase(opponentsShots.begin() + i);
				i--;
				shotDeleted = true;
			}

			if (shotDeleted) continue;

			if (arena->PlayerCollidesWithShot(shot)) {
				gameOver = 1;
				delete shot;
				opponentsShots.erase(opponentsShots.begin() + i);
				i--;
				shotDeleted = true;
			}

			if (shotDeleted) continue;

			if (!shot->Valid()) {
				delete shot;
				opponentsShots.erase(opponentsShots.begin() + i);
				i--;
			}
		}
	}

	// Check if the player won
	if (arena->PlayerWon()) { playerWon = 1; }

	// Redraw the scene
	glutPostRedisplay();
}



/*****************************************************************************************/
/************************************* MAIN FUNCTION *************************************/
/*****************************************************************************************/

int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("Usage: %s <svg file>\n", argv[0]);
		exit(1);
	}

	svgFilePath = argv[1];

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(Width, Height);
	glutInitWindowPosition(150, 50);
	glutCreateWindow("Trabalho 3D");

	glutDisplayFunc(renderScene);
	glutKeyboardFunc(keyPress);
	glutIdleFunc(idle);
	glutKeyboardUpFunc(keyUp);
	glutPassiveMotionFunc(passiveMotion);
	glutMotionFunc(mouseMotion);
	glutMouseFunc(mouseClick);
	
	init(Width, svgFilePath);

	glutMainLoop();

	return 0;
}
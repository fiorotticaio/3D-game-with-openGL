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

// Camera
GLfloat cameraX = xPositionArena, cameraY = yPositionArena, cameraZ = 100;
GLfloat lookAtX = 0, lookAtY = 0, lookAtZ = 0;
GLfloat upX = 0, upY = 1, upZ = 0;

// Components of the virtual world
Arena* arena = NULL;
std::vector<Shot*> playerShots;
std::vector<Shot*> opponentsShots;

// Flags and aux variables
char* svgFilePath = NULL;
float positionTolerance = 0.5f;
float mouseY = 0.0f;
GLfloat timeAccumulator = 0.0f;
void* font = GLUT_BITMAP_9_BY_15;
int gameOver = 0;
int playerWon = 0;

// Feature flags
int simulateSlowProcessingUbuntu = 0;
int simulateSlowProcessingWindows = 0;
int opponentMoves = 0;
int opponentShoots = 0;



/*****************************************************************************************/
/************************************ AUX FUNCTIONS **************************************/
/*****************************************************************************************/
void PrintMessage(const char* message, GLfloat x, GLfloat y, GLfloat R, GLfloat G, GLfloat B) {
	glColor3f(R, G, B);
	glRasterPos2f(x, y);

	char* tempStr = (char*) message;
	while (*tempStr) {
		glutBitmapCharacter(font, *tempStr);
		tempStr++;
	}
}


void UpdateViewport(GLfloat playerX, GLfloat playerY, 
                    GLfloat arenaX, GLfloat arenaY, 
                    GLfloat arenaWidth, GLfloat arenaHeight, 
                    GLfloat viewingWidth, GLfloat viewingHeight) {
    GLfloat newViewportX, newViewportY;

    GLfloat centeredX = playerX - viewingWidth / 2;

    if (centeredX < arenaX) { // Min limit
        newViewportX = arenaX;
    } else if (centeredX + viewingWidth > arenaX + arenaWidth) { // Max limit
        newViewportX = arenaX + arenaWidth - viewingWidth;
    } else { // Normal case
        newViewportX = centeredX;
    }

    GLfloat centeredY = playerY - viewingHeight / 2;

    if (centeredY < arenaY) { // Min limit
        newViewportY = arenaY;
    } else if (centeredY + viewingHeight > arenaY + arenaHeight) { // Max limit
        newViewportY = arenaY + arenaHeight - viewingHeight;
    } else { // Normal case
        newViewportY = centeredY;
    }

    // Update the viewport
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(newViewportX,                       
            newViewportX + viewingWidth,        
            newViewportY,                       
            newViewportY + viewingHeight,       
            -100,                               
            100);                               
    glMatrixMode(GL_MODELVIEW);

	viewPortLeft = newViewportX;
	viewPortRight = newViewportX + viewingWidth;
	viewPortBottom = newViewportY;
	viewPortTop = newViewportY + viewingHeight;

    glLoadIdentity();
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
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Center the camera in the player
	lookAtX = arena->GetPlayerGx();
	lookAtY = arena->GetPlayerGy();
	lookAtZ = 0;
	cameraX = lookAtX;
	cameraY = lookAtY;
	// cameraZ = 100;


	// Put the camera in the correct position
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraX, cameraY, cameraZ, lookAtX, lookAtY, lookAtZ, upX, upY, upZ);
	
	// Set the light 0 position
	GLfloat light_position[] = {arena->GetPlayerGx(), arena->GetPlayerGy()+10, 0.0, 1.0};
	glPushMatrix();
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glPopMatrix();
    
	arena->Draw();

	for (Shot* shot : playerShots) {
		if (shot) shot->Draw();
	}

	for (Shot* shot : opponentsShots) {
		if (shot) shot->Draw();
	}

	if (gameOver) {
		GLfloat messagePosX = viewPortLeft + viewingWidth / 2 - 6;
		GLfloat messagePosy = viewPortBottom + viewingHeight / 2 + 15;
		PrintMessage("Game Over", messagePosX, messagePosy, 1.0f, 0.0f, 0.0f);
	}

	if (playerWon) {
		GLfloat messagePosX = viewPortLeft + viewingWidth / 2 - 6;
		GLfloat messagePosy = viewPortBottom + viewingHeight / 2 + 15;
		PrintMessage("Player Won", messagePosX, messagePosy, 1.0f, 1.0f, 1.0f);
	}

	// Draw on the frame buffer
	glutSwapBuffers(); // Function suitable for double buffer window
}


void keyPress(unsigned char key, int x, int y) {
	switch (key) {
		case '1':
			simulateSlowProcessingUbuntu = !simulateSlowProcessingUbuntu;
			break;
		case '2':
			simulateSlowProcessingWindows = !simulateSlowProcessingWindows;
			break;
		case '3':
			opponentMoves = !opponentMoves;
			break;
		case '4':
			opponentShoots = !opponentShoots;
			break;
		case '5':
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
		case 'y':
		case 'Y':
			keyStatus[(int)('y')] = 1;
			break;
		case ' ':
			keyStatus[(int)(' ')] = 1;
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


void init(int windowSize) {
	glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, (GLsizei) windowSize, (GLsizei) windowSize);

	// Defining camera parameters
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90, (GLfloat) windowSize / (GLfloat) windowSize, 1, 200);

	ResetKeyStatus();

	// The color the windows will redraw. Its done to erase the previous frame
	// glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black, no opacity (alpha)

	// glMatrixMode(GL_PROJECTION); // Select the projection matrix    
	// glOrtho(xPositionArena,                 // X coordinate of left edge             
	// 		xPositionArena + viewingWidth,  // X coordinate of right edge            
	// 		yPositionArena,                 // Y coordinate of bottom edge             
	// 		yPositionArena + viewingHeight, // Y coordinate of top edge             
	// 		-100,                           // Z coordinate of the “near” plane            
	// 		100);                           // Z coordinate of the “far” plane
	// glMatrixMode(GL_MODELVIEW); // Select the projection matrix    

	// glLoadIdentity();
}


void passiveMotion(int x, int y) {
	// Invert the y position
	mouseY = Height - y;
}


void mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && !gameOver && !playerWon) {
        playerShots.push_back(arena->PlayerShoot(viewingWidth));
    }
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN && arena->PlayerLanded()) {
		arena->PlayerJump();
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
		arena->SetPlayerYDirection(-1);
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
    
	arena = new Arena(svgFilePath);

    // UpdateViewport(arena->GetPlayerGx(), arena->GetPlayerGy(), 
    //                xPositionArena, yPositionArena, 
    //                arenaWidth, arenaHeight, 
    //                viewingWidth, viewingHeight);
}


void idle(void) {
	if (keyStatus[(int)('y')]) {
		cameraZ += 0.5;
	}
	if (keyStatus[(int)('h')]) {
		cameraZ -= 0.5;
	}

	if (simulateSlowProcessingUbuntu) for (int i = 0; i < 9000000; i++);
	if (simulateSlowProcessingWindows) for (int i = 0; i < 90000000; i++);

	static GLdouble previousTime = glutGet(GLUT_ELAPSED_TIME);
	GLdouble currentTime, timeDifference;
	currentTime = glutGet(GLUT_ELAPSED_TIME);   // Get the time that has passed since the start of the application
	timeDifference = currentTime - previousTime; // Calculates the elapsed time since the last frame
	previousTime = currentTime;                 // Update the time of the last frame that occurred

	// Avoids the program to crash when the difference is too high
	if (timeDifference <= 0.0f || timeDifference > 200.0f) timeDifference = 1.0f;

	timeAccumulator += timeDifference;
	
	if (keyStatus[(int)('r')] && (gameOver || playerWon)) {
		ResetGame();
	}

	if (gameOver || playerWon) return;


	if (keyStatus[(int)('a')]) {
		arena->SetPlayerXDirection(-1);
		arena->MovePlayerInX(timeDifference);
	}
	if (keyStatus[(int)('d')]) {
		arena->SetPlayerXDirection(1);
		arena->MovePlayerInX(timeDifference);
	}

	arena->RotatePlayerArm(mouseY, Height, timeDifference);
	arena->MovePlayerInY(timeDifference);
	arena->MoveOpponentsInY(timeDifference);
	if (opponentMoves) arena->MoveOpponentsInX(timeDifference);
	arena->MoveOpponentsArms(timeDifference);

	if (timeAccumulator >= 1000.0f) {
		if (opponentShoots) arena->UpdateOpponentsShots(opponentsShots, arenaWidth, timeDifference);
		timeAccumulator = 0.0f;
	}

	// UpdateViewport(arena->GetPlayerGx(), arena->GetPlayerGy(), 
	// 			   xPositionArena, yPositionArena,
	// 			   arena->GetWidth(), arena->GetHeight(),
	// 			   viewingWidth, viewingHeight);
	
	if (arena->PlayerReachedMaximumJumpHeight() || arena->PlayerHitsHead()) {
		arena->SetPlayerYDirection(-1);
	}
	
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

	if (arena->PlayerWon()) { playerWon = 1; }

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

	if (!loadViewportSizeFromSvg(svgFilePath)) {
		exit(1);
	}

	// Initialize the arena
	arena = new Arena(svgFilePath);

	// Initialize openGL with Double buffer and RGB color without transparency.
	// Its interesting to try GLUT_SINGLE instead of GLUT_DOUBLE.
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	// Create the window
	glutInitWindowSize(Width, Height);
	glutInitWindowPosition(150, 50);
	glutCreateWindow("Trabalho 3D");

	// Define callbacks
	glutDisplayFunc(renderScene);
	glutKeyboardFunc(keyPress);
	glutIdleFunc(idle);
	glutKeyboardUpFunc(keyUp);
	glutPassiveMotionFunc(passiveMotion);
	glutMouseFunc(mouseClick);
	
	init(Width);

	glutMainLoop();

	return 0;
}
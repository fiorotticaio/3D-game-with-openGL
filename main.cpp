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
#include "camera.h"

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
Camera* firstPersonCamera = NULL;
Camera* sightCamera = NULL;
Camera* thirdPersonCamera = NULL;
double camXYAngle = 0;
double camXZAngle = 0;
int lastX = 0;
int lastY = 0;
int zoom = 50;

// Flags and aux variables
char* svgFilePath = NULL;
float positionTolerance = 0.5f;
float mouseY = 0.0f;
GLfloat timeAccumulator = 0.0f;
void* font = GLUT_BITMAP_9_BY_15;
int gameOver = 0;
int playerWon = 0;
int toggleCam = 1;

// Feature flags
int simulateSlowProcessingUbuntu = 0;
int simulateSlowProcessingWindows = 0;
int opponentMoves = 0;
int opponentShoots = 0;



/*****************************************************************************************/
/************************************ AUX FUNCTIONS **************************************/
/*****************************************************************************************/
void normalize(float a[3]) {
    double norm = sqrt(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]); 
    a[0] /= norm;
    a[1] /= norm;
    a[2] /= norm;
}


void cross(float a[3], float b[3], float out[3]) {
    out[0] = a[1]*b[2] - a[2]*b[1];
    out[1] = a[2]*b[0] - a[0]*b[2];
    out[2] = a[0]*b[1] - a[1]*b[0];
}


void RasterChars(GLfloat x, GLfloat y, GLfloat z, const char * text, double r, double g, double b) {
    glPushAttrib(GL_ENABLE_BIT);
        glDisable(GL_LIGHTING);
        // glDisable(GL_TEXTURE_2D);

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


void PrintText(GLfloat x, GLfloat y, const char *text, double r, double g, double b) {
    glMatrixMode (GL_PROJECTION);
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
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (toggleCam == 1){
        PrintText(0.1, 0.1, "First person camera", 0, 1, 0);

		glTranslatef(0, 0, -zoom);
		glRotatef(camXZAngle, 1, 0, 0);
		glRotatef(camXYAngle, 0, 1, 0);
		// We want that this next translation do not interfere with the camera rotation
		glTranslatef(-arena->GetPlayerGx(), -arena->GetPlayerGy(), -arena->GetPlayerGz());
 
    } else if (toggleCam == 2){
        PrintText(0.1, 0.1, "Gun sight camera", 0, 1, 0);

    } else if (toggleCam == 3){
        PrintText(0.1, 0.1, "Third person camera", 0, 1, 0);

    }

	// A partir daqui estamos no sistema de coordenadas do mundo
	
	GLfloat light_position[] = {arena->GetGx()+arena->GetWidth()/2, 
								arena->GetGy()+arena->GetHeight()-20, 
								-arena->GetThickness()/2, 
								1.0}; // Last element 1.0 means it is a point light

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	
	glPushMatrix();
		glTranslatef(arena->GetPlayerGx(), arena->GetPlayerGy(), arena->GetPlayerGz());
		DrawAxes();
	glPopMatrix();

	arena->Draw();

	for (Shot* shot : playerShots) {
		if (shot) shot->Draw();
	}

	for (Shot* shot : opponentsShots) {
		if (shot) shot->Draw();
	}

	if (gameOver) {
		GLfloat messagePosX = 0.5;
		GLfloat messagePosy = 0.5;
		PrintText(messagePosX, messagePosy, "Game Over", 1, 0, 0);		
	}
	
	if (playerWon) {
		GLfloat messagePosX = 0.5;
		GLfloat messagePosy = 0.5;
		PrintText(messagePosX, messagePosy, "Player Won", 1, 1, 1);
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
		case '+':
			zoom--;
			break;
		case '-':
			zoom++;
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
    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHT0);
	// glEnable(GL_TEXTURE_2D)
	glDepthFunc(GL_LEQUAL);

    glViewport(0, 0, (GLsizei) windowSize, (GLsizei) windowSize);

	// Defining camera parameters
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90, (GLfloat) windowSize / (GLfloat) windowSize, 1, 150);

	ResetKeyStatus();
}


void passiveMotion(int x, int y) {
	// Invert the y position
	mouseY = Height - y;

	camXYAngle += x - lastX;
    camXZAngle += y - lastY;
    
    camXYAngle = (int) camXYAngle % 360;
    camXZAngle = (int) camXZAngle % 360;
    
    lastX = x;
    lastY = y;
}


void mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && !gameOver && !playerWon) {
        playerShots.push_back(arena->PlayerShoot(viewingWidth));
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
	if (keyStatus[(int)(' ')] && arena->PlayerLanded()) {
		arena->PlayerJump();
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
	
	if (!keyStatus[(int)(' ')] || arena->PlayerReachedMaximumJumpHeight() || arena->PlayerHitsHead()) {
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

	arena = new Arena(svgFilePath);

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
	glutMouseFunc(mouseClick);
	
	init(Width);

	glutMainLoop();

	return 0;
}
// Graphics1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <glut.h>
#include <string>

using namespace std;

#define SPACEBAR 32
#define R 114

const int WINDOW_HEIGHT = 600;
const int WINDOW_WIDTH = 900;
const int BORDER_WIDTH = 20;
const int FPS = 33; // On internet they argue that this is 30 FPS :D
const int JUMP_HEIGHT = 30;
const int DEFAULT_X_SPEED = 7;



void render();
void drawCircle(int, int, float);
void timer(int);
void keyboardHandler(unsigned char, int, int);
void drawBitmapText(char, float, float, float);
void resetPosition();

float timeElapsed = 0;
bool gameOver = false;

class Player {
private:
	int xPosition, yPosition;
	int width, height;
	float xSpeed;
	float ySpeed;
	float xAcceleration;
	float yAcceleration;

public:
	Player(int, int, int, int);
	void update();
	void draw();
	void setXPosition(int x) { this->xPosition = x; }
	void setYPosition(int y) { this->yPosition = y; }
	void setXSpeed(int x) { this->xSpeed = x; }
	void setRelativeXSpeed(int x);

	void setYSpeed(int y) { this->ySpeed = y; }
	float getXSpeed() { return this->xSpeed; }
	float getYSpeed() { return this->ySpeed; }

	float getXPosition() { return this->xPosition; }
	float getYPosition() { return this->yPosition; }

	int getWidth() { return this->width; }
	int getHeight() { return this->height; }

	void setHeight(int height) { this->height = height; }
	void setWidth(int width) { this->width = width; }

	bool rightTouch() { return this->xPosition >= WINDOW_WIDTH - BORDER_WIDTH - this->width; }
	bool leftTouch() { return this->xPosition <= BORDER_WIDTH; }

	bool onFirstLevel();
};


/*
 * Player's constructor
 */
Player::Player(int xPosition, int yPosition, int width, int height) {
	this->xPosition = xPosition;
	this->yPosition = yPosition;
	this->width = width;
	this->height = height;
	this->xSpeed = DEFAULT_X_SPEED;
	this->yAcceleration = -3;

	cout << "Player Created at ";
	cout << "(x: " << this->xPosition << ", ";
	cout << "y: " << this->yPosition << ")";
	cout << "\nWidth : " << this->width;
	cout << "\nHeight : " << this->height;
	cout << "\n--------------------------\n";
}

Player p1(BORDER_WIDTH, 0, 50, 50);

// Constants that depends on player's properties 
const int LEVEL_ONE_START = BORDER_WIDTH + 100 + p1.getWidth();
const int LEVEL_ONE_END = WINDOW_WIDTH - BORDER_WIDTH;
const int LEVEL_ONE_HEIGHT = p1.getHeight() + 185;
const int LEVEL_ONE_THIKNESS = LEVEL_ONE_HEIGHT + 15;

const int DOOR_START = BORDER_WIDTH;
const int DOOR_END = DOOR_START + 20;
const int DOOR_HEIGHT = LEVEL_ONE_THIKNESS + 180;

const int OBSTACLE_WIDTH = 20;
const int OBSTACLE_HEIGHT = 40;
const int OBSTACLE_1_X_START = 500;
const int OBSTACLE_1_Y_START = 0;
const int OBSTACLE_2_X_START = 400;
const int OBSTACLE_2_Y_START = LEVEL_ONE_THIKNESS;

void Player::draw() {
	glPushMatrix();
	glColor3f(0.0f, 0.0f, 0.0f);

	//Body of the player
	glBegin(GL_QUADS);
	glVertex3f(this->xPosition, this->yPosition, 0.0f);
	glVertex3f(this->xPosition + this->width, this->yPosition, 0.0f);
	glVertex3f(this->xPosition + this->width, this->yPosition + this->height, 0.0f);
	glVertex3f(this->xPosition, this->yPosition + this->height, 0.0f);
	glEnd();

	float eyeOffsetX = 0.15 * this->width;
	float eyeOffsetY = 0.10 * this->height;
	float eyeSize = 0.20 * this->width;


	// Left Eye
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_POLYGON);

	glVertex3f(this->xPosition + eyeOffsetX, this->yPosition + this->height - (eyeOffsetY + eyeSize), 0.0f);
	glVertex3f(this->xPosition + eyeOffsetX + eyeSize, this->yPosition + this->height - (eyeOffsetY + eyeSize), 0.0f);
	glVertex3f(this->xPosition + eyeOffsetX + eyeSize, this->yPosition + this->height - eyeOffsetY, 0.0f);
	glVertex3f(this->xPosition + eyeOffsetX + (eyeSize / 2), this->yPosition + this->height - (eyeOffsetY + (eyeSize / 4)), 0.0f);
	glVertex3f(this->xPosition + eyeOffsetX, this->yPosition + this->height - eyeOffsetY, 0.0f);

	glEnd();

	// Left Lens
	glColor3f(1.0f, 0.0f, 0.0f);
	drawCircle(this->xPosition + eyeOffsetX + (eyeSize / 2), this->yPosition + this->height - (eyeOffsetY + eyeSize / 1.5), 1.3f);


	// Right Eye
	glBegin(GL_POLYGON);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(this->xPosition + eyeOffsetX + (this->width / 2), this->yPosition + this->height - (eyeOffsetY + eyeSize), 0.0f);
	glVertex3f(this->xPosition + eyeOffsetX + (this->width / 2) + eyeSize, this->yPosition + this->height - (eyeOffsetY + eyeSize), 0.0f);
	glVertex3f(this->xPosition + eyeOffsetX + (this->width / 2) + eyeSize, this->yPosition + this->height - eyeOffsetY, 0.0f);
	glVertex3f(this->xPosition + eyeOffsetX + (this->width / 2) + (eyeSize / 2), this->yPosition + this->height - (eyeOffsetY + (eyeSize / 4)), 0.0f);
	glVertex3f(this->xPosition + eyeOffsetX + (this->width / 2), this->yPosition + this->height - eyeOffsetY, 0.0f);

	glEnd();

	// Right Lens
	glColor3f(1.0f, 0.0f, 0.0f);
	drawCircle(this->xPosition + eyeOffsetX + (this->width / 2) + (eyeSize / 2), this->yPosition + this->height - (eyeOffsetY + eyeSize / 1.5), 1.3f);


	glPopMatrix();
}

void Player::update() {
	this->xPosition += this->xSpeed;

	if (!this->onFirstLevel()) {
		this->ySpeed += this->yAcceleration;
		this->yPosition += this->ySpeed;

		if (this->xPosition >= LEVEL_ONE_START - this->width && this->xPosition <= LEVEL_ONE_END + 10) { // if he is under the level
			if (this->yPosition >= LEVEL_ONE_HEIGHT - this->height && this->yPosition <= LEVEL_ONE_THIKNESS - 10) {
				this->ySpeed = 0;
				this->yPosition = LEVEL_ONE_HEIGHT - this->height;
			}
		}
	}
	else { // if 3antar is on the first level
		this->yPosition += this->ySpeed;
		if (this->yPosition <= LEVEL_ONE_THIKNESS) {
			this->yPosition = LEVEL_ONE_THIKNESS;
		}
		this->setRelativeXSpeed(DEFAULT_X_SPEED);
	}

	// Bouncing if touched borders
	if (this->rightTouch() || this->leftTouch()) {
		this->xSpeed *= -1;
	}

	// if 3antar hits the ground
	if (this->yPosition <= 0) {
		this->yPosition = 0;
		this->setRelativeXSpeed(DEFAULT_X_SPEED);
	}

	if (this->xPosition <= DOOR_END && this->yPosition >= DOOR_HEIGHT - this->height && this->yPosition <= DOOR_HEIGHT + 50) {
		gameOver = true;
	}

	// Checking for obstacles collisions
	if (((this->xPosition + this->width >= OBSTACLE_1_X_START && this->xPosition + this->width <= OBSTACLE_1_X_START + OBSTACLE_WIDTH)
		|| ((this->xPosition >= OBSTACLE_1_X_START && this->xPosition <= OBSTACLE_1_X_START + OBSTACLE_WIDTH)))
		&& ((this->yPosition >= OBSTACLE_1_Y_START && this->yPosition <= OBSTACLE_1_Y_START + OBSTACLE_HEIGHT)) ||
		((this->xPosition + this->width >= OBSTACLE_2_X_START && this->xPosition + this->width <= OBSTACLE_2_X_START + OBSTACLE_WIDTH)
			|| ((this->xPosition >= OBSTACLE_2_X_START && this->xPosition <= OBSTACLE_2_X_START + OBSTACLE_WIDTH)))
		&& ((this->yPosition >= OBSTACLE_2_Y_START && this->yPosition <= OBSTACLE_2_Y_START + OBSTACLE_HEIGHT))) {

		resetPosition();
	}


	glutPostRedisplay();
}

bool Player::onFirstLevel() {
	if (this->xPosition >= LEVEL_ONE_START - this->width && this->xPosition <= LEVEL_ONE_END + 10) {
		if (this->yPosition >= LEVEL_ONE_THIKNESS && this->yPosition <= LEVEL_ONE_THIKNESS + this->height) {
			return true;
		}
	}
	return false;
}

void Player::setRelativeXSpeed(int x) {
	if (this->xSpeed > 0) {
		this->xSpeed = x;
	}
	else {
		this->xSpeed = -1 * x;
	}
}



/*
 * Drawing the borders of the map
 */
void drawBorders() {

	glPushMatrix();
	glColor3f(0.407843f, 0.407843f, 0.407843f);

	// Left Border
	glBegin(GL_QUADS);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(BORDER_WIDTH, 0.0f, 0.0f);
	glVertex3f(BORDER_WIDTH, WINDOW_HEIGHT, 0.0f);
	glVertex3f(0.0f, WINDOW_HEIGHT, 0.0f);
	glEnd();


	// Right Border
	glBegin(GL_QUADS);
	glVertex3f(WINDOW_WIDTH - BORDER_WIDTH, 0.0f, 0.0f);
	glVertex3f(WINDOW_WIDTH, 0.0f, 0.0f);
	glVertex3f(WINDOW_WIDTH, WINDOW_HEIGHT, 0.0f);
	glVertex3f(WINDOW_WIDTH - BORDER_WIDTH, WINDOW_HEIGHT, 0.0f);
	glEnd();

	glPopMatrix();

}

void drawDoor() {
	glColor3f(0.0039215f, 0.7647058f, 0.560784f);
	glBegin(GL_QUADS);
	glVertex3f(DOOR_START, DOOR_HEIGHT, 0.0f);
	glVertex3f(DOOR_END, DOOR_HEIGHT, 0.0f);
	glVertex3f(DOOR_END, DOOR_HEIGHT + 50, 0.0f);
	glVertex3f(DOOR_START, DOOR_HEIGHT + 50, 0.0f);
	glEnd();
}

void drawObstacles() {
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(OBSTACLE_1_X_START, OBSTACLE_1_Y_START, 0.0f);
	glVertex3f(OBSTACLE_1_X_START + OBSTACLE_WIDTH, OBSTACLE_1_Y_START, 0.0f);
	glVertex3f(OBSTACLE_1_X_START + OBSTACLE_WIDTH, OBSTACLE_1_Y_START + OBSTACLE_HEIGHT, 0.0f);
	glVertex3f(OBSTACLE_1_X_START, OBSTACLE_1_Y_START + OBSTACLE_HEIGHT, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(OBSTACLE_2_X_START, OBSTACLE_2_Y_START, 0.0f);
	glVertex3f(OBSTACLE_2_X_START + OBSTACLE_WIDTH, OBSTACLE_2_Y_START, 0.0f);
	glVertex3f(OBSTACLE_2_X_START + OBSTACLE_WIDTH, OBSTACLE_2_Y_START + OBSTACLE_HEIGHT, 0.0f);
	glVertex3f(OBSTACLE_2_X_START, OBSTACLE_2_Y_START + OBSTACLE_HEIGHT, 0.0f);
	glEnd();
}


void drawLevel() {
	drawBorders();
	drawDoor();
	drawObstacles();

	glPushMatrix();
	glColor3f(0.5882f, 0.0980f, 0.0549f);

	// TODO: Make it relative to the jump height
	// First Level
	glBegin(GL_QUADS);
	glVertex3f(LEVEL_ONE_START, LEVEL_ONE_THIKNESS, 0.0f);
	glVertex3f(LEVEL_ONE_END, LEVEL_ONE_THIKNESS, 0.0f);
	glVertex3f(LEVEL_ONE_END, LEVEL_ONE_HEIGHT, 0.0f);
	glVertex3f(LEVEL_ONE_START, LEVEL_ONE_HEIGHT, 0.0f);
	glEnd();

	glPopMatrix();

}

void drawCircle(int x, int y, float r) {
	glPushMatrix();
	glTranslatef(x, y, 0);
	GLUquadric *quadObj = gluNewQuadric();
	gluDisk(quadObj, 0, r, 50, 50);
	glPopMatrix();
}

/*
 * Rendring text to screen
 */
void drawBitmapText(string text, float x, float y, float z)
{

	glColor3f(0.0, 0.0, 0.0);
	glRasterPos3f(x, y, z);

	for (int i = 0; i < text.size(); i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
	}
}

void drawTimer() {
	string timeString = "Time : " + to_string(int(timeElapsed)) + " s";
	drawBitmapText(timeString, 760, 550, 0);
}

void resetPosition() {
	p1.setXPosition(BORDER_WIDTH);
	p1.setYPosition(0);
	p1.setHeight(50);
	p1.setWidth(50);

	p1.setXSpeed(DEFAULT_X_SPEED);
	p1.setYSpeed(0);
}


void main(int argc, char** argr) {
	glutInit(&argc, argr);

	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(50, 50);

	glutCreateWindow("Graphics Assignment");
	glutDisplayFunc(render);
	glutTimerFunc(0, timer, 0);
	glutKeyboardFunc(keyboardHandler);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	gluOrtho2D(0.0, WINDOW_WIDTH, 0.0, WINDOW_HEIGHT);

	glutMainLoop();

}

void timer(int t) {
	p1.update();
	if (!gameOver) {
		timeElapsed += FPS / 1000.0;
	}
	glutTimerFunc(FPS, timer, 0);
}

void keyboardHandler(unsigned char key, int x, int y) {
	switch (key)
	{
	case SPACEBAR:
		// Jump
		if (p1.getYPosition() <= 0 || (p1.onFirstLevel() && p1.getYPosition() <= LEVEL_ONE_THIKNESS + 5)) { // dirty fix !
			p1.setYSpeed(JUMP_HEIGHT);
		}

		// Double jump
		if ((p1.getXPosition() >= WINDOW_WIDTH - BORDER_WIDTH - p1.getWidth() - 15)
			|| (p1.getXPosition() <= BORDER_WIDTH + 15)) {
			p1.setYSpeed(JUMP_HEIGHT);
		}

		// Speed 3antar in x while jumping
		p1.setRelativeXSpeed(13);

		break;

	case R:
		if (gameOver) {
			resetPosition();
			gameOver = false;
			timeElapsed = 0;
		}

	default:
		break;
	}
}

void render() {
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	if (!gameOver) {
		drawLevel();
		p1.draw();

		drawTimer();
	}
	else {
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		drawBitmapText("You are out!", 400, 300, 0);
		drawBitmapText("You managed to escape in " + to_string(int(timeElapsed)) + " seconds !", 305, 270, 0);
		drawBitmapText("Press R to restart", 385, 230, 0);
	}

	glFlush();
}
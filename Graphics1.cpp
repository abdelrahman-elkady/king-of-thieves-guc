// Graphics1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <glut.h>
using namespace std;

#define SPACEBAR 32

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
	void setRelativeYSpeed(int y);
	void setYSpeed(int y) { this->ySpeed = y; }
	float getXSpeed() { return this->xSpeed; }
	float getYSpeed() { return this->ySpeed; }

	float getXPosition() { return this->xPosition; }
	float getYPosition() { return this->yPosition; }

	int getWidth() { return this->width; }
	int getHeight() { return this->height; }

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
	}
	else { // if 3antar is on the first level
		this->yPosition += this->ySpeed;
		if (this->yPosition <= this->height + 195) {
			this->yPosition = this->height + 195;
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

	this->onFirstLevel();


	glutPostRedisplay();
}

bool Player::onFirstLevel() {

	if (this->xPosition >= BORDER_WIDTH + 100 + this->getWidth() - 10 && this->xPosition <= BORDER_WIDTH + 400 + this->getWidth() + 10) {
		if (this->yPosition >= this->height + 195 && this->yPosition <= this->height + 205) {
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


Player p1(BORDER_WIDTH, 0, 50, 50);


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


void drawLevel() {
	drawBorders();

	glPushMatrix();
	glColor3f(0.5882f, 0.0980f, 0.0549f);

	// TODO: Make it relative to the jump height
	glBegin(GL_QUADS);
	glVertex3f(BORDER_WIDTH + 100 + p1.getWidth(), p1.getHeight() + 195, 0.0f);
	glVertex3f(BORDER_WIDTH + 400 + p1.getWidth(), p1.getHeight() + 195, 0.0f);
	glVertex3f(BORDER_WIDTH + 400 + p1.getWidth(), p1.getHeight() + 180, 0.0f);
	glVertex3f(BORDER_WIDTH + 100 + p1.getWidth(), p1.getHeight() + 180, 0.0f);
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

	glutTimerFunc(FPS, timer, 0);
}

void keyboardHandler(unsigned char key, int x, int y) {
	switch (key)
	{
	case SPACEBAR:
		// Jump
		if (p1.getYPosition() <= 0 || p1.onFirstLevel()) {
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

	default:
		break;
	}
}

void render() {
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	drawLevel();
	p1.draw();

	glFlush();
}
// Graphics1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <glut.h>
using namespace std;

const int WINDOW_HEIGHT = 600;
const int WINDOW_WIDTH = 900;
const int BORDER_WIDTH = 20;
const int FPS = 33; // On internet they argue that this is 30 FPS :D

void render();
void drawCircle(int, int, float);
void timer(int);

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

};


/*
 * Player's constructor
 */
Player::Player(int xPosition, int yPosition, int width, int height) {
	this->xPosition = xPosition;
	this->yPosition = yPosition;
	this->width = width;
	this->height = height;
	this->xSpeed = 7;

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

	if (this->xPosition >= WINDOW_WIDTH - BORDER_WIDTH - this->width || this->xPosition <= BORDER_WIDTH) {
		this->xSpeed *= -1;
	}

	cout << xPosition << "\n";

	glutPostRedisplay();
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

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	gluOrtho2D(0.0, WINDOW_WIDTH, 0.0, WINDOW_HEIGHT);

	glutMainLoop();

}

Player p1(BORDER_WIDTH, 0, 50, 50);


void timer(int t) {
	p1.update();

	glutTimerFunc(FPS, timer, 0);
}

void render() {
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	p1.draw();
	drawBorders();

	glFlush();
}
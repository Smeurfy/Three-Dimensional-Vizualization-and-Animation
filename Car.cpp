#include"Car.h"
#include<cmath>

#define PI 3.14159265358979323846

Car::Car(float x, float y, float z, std::string _name) {
	carX = x;
	carY = y;
	carZ = z;
	name = _name;
}
float Car::getCarX() {
	return carX;
}
void Car::setCarX(float x) {
	carX = x;
}
float Car::getCarY() {
	return carY;
}
void Car::setCarY(float y) {
	carY = y;
}
float Car::getCarZ() {
	return carZ;
}
void Car::setCarZ(float z) {
	carZ = z;
}
float Car::getCollisionRadius() {
	return COLRADIUSC;
}
float Car::getSpeed() {
	return speed;
}
void Car::setSpeed(float s) {
	speed = s;
}
float Car::getAccFront() {
	return maxAccFront;
}
float Car::getAccBack() {
	return maxAccBack;
}
float Car::getAcceleration() {
	return acceleration;
}
void Car::setAcceleration(float acc) {
	acceleration = acc;
}
bool Car::getMoveForward() {
	return moveForward;
}
void Car::setMoveForward(bool forward) {
	moveForward = forward;
}
bool Car::getMoveBack() {
	return moveBack;
}
void Car::setMoveBack(bool back) {
	moveBack = back;
}
bool Car::getMoveRight() {
	return moveRight;
}
void Car::setMoveRight(bool r) {
	moveRight = r;
}
bool Car::getMoveLeft() {
	return moveLeft;
}
void Car::setMoveLeft(bool l) {
	moveLeft = l;
}
float Car::getRotation() {
	return rotation;
}
void Car::setRotation(float angle) {
	rotation = angle;
}
bool Car::getForwardFriction() {
	return forwardFriction;
}
void Car::setForwardFriction(bool l) {
	forwardFriction = l;
}
bool Car::getBackFriction() {
	return backFriction;
}
void Car::setBackFriction(bool l) {
	backFriction = l;
}
float Car::getMovFrictionFront() {
	return movFrictionFront;
}
float Car::getMovFrictionBack() {
	return movFrictionBack;
}
bool Car::getHasCollisionF() {
	return hasCollisionF;
}
void Car::setHasCollisionF(bool col) {
	hasCollisionF = col;
}
bool Car::getHasCollisionB() {
	return hasCollisionB;
}
void Car::setHasCollisionB(bool col) {
	hasCollisionB = col;
}
void Car::update(float time) {
	float posX = carX + speed * time + 0.5*acceleration*time*time;
	float posY = carY + speed * time + 0.5*acceleration*time*time;
	float disX = posX - carX;
	float disY = posY - carY;
	float vel = speed + acceleration * time;

	this->setScore(1); // 1 point for each time the function is called

	if (moveForward) {
		if (vel >= maxSpeedFront) {
			speed = maxSpeedFront;
		}
		if (moveBack) {
			speed = vel;
			acceleration = handBrake;
			if (speed < 0) {
				acceleration = 0;
				speed = 0;
			}
		}
		if (moveRight && speed != 0) {
			rotation -= 3 * PI/180;
		}
		else if (moveLeft && speed != 0) {
			rotation += 3 * PI/180;
		}
		carX += disX * cos(rotation);
		carY += disY * sin(rotation);
	}
	else if (forwardFriction) {
		speed = vel;
		if (speed < 0) {
			speed = 0;
			acceleration = 0;
			forwardFriction = false;
		}
		if (moveRight && speed != 0) {
			rotation -= 3 * PI / 180;
		}
		else if (moveLeft && speed != 0) {
			rotation += 3 * PI / 180;
		}
		carX += disX * cos(rotation);
		carY += disY * sin(rotation);
	}
	else if (moveBack) {
		if (vel <= maxSpeedBack) {
			speed = maxSpeedBack;
		}
		if (moveRight && speed != 0) {
			rotation -= 3 * PI / 180;
		}
		else if (moveLeft && speed != 0) {
			rotation += 3 * PI / 180;
		}

		carX += disX * cos(rotation);
		carY += disY * sin(rotation);
	}
	else if (backFriction) {
		speed = vel;
		if (speed > 0) {
			speed = 0;
			acceleration = 0;
			backFriction = false;
		}
		if (moveRight && speed != 0) {
			rotation -= 3 * PI / 180;
		}
		else if (moveLeft && speed != 0) {
			rotation += 3 * PI / 180;
		}
		carX += disX * cos(rotation);
		carY += disY * sin(rotation);
	}
}

int Car::getLives() {
	return lives;
}

void Car::decLives() {
	int aux = lives;
	if (lives != 0)
		lives--;
}

unsigned long Car::getScore() {
	return score;
}
void Car::setScore(int value) {
	score += value;
}
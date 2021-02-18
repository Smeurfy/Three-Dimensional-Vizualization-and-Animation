
#include"Orange.h"
//#include"GameObject.h"
#include<cmath>
#include<math.h>

#define PI 3.14159265358979323846

Orange::Orange(float x, float y, float z, std::string _name) {
	orangeX = x;
	orangeY = y;
	orangeZ = z;
	name = _name;
}
float Orange::getOrangeX() {
	return orangeX;
}
float Orange::getOrangeY() {
	return orangeY;
}
float Orange::getOrangeZ() {
	return orangeZ;
}
float Orange::getCollisionRadius() {
	return COLRADIUSO;
}
float Orange::getSpeedX() {
	return speedX;
}
float Orange::getSpeedY() {
	return speedY;
}
void Orange::setSpeedX(float speed) {
	speedX = speed;
}
void Orange::setSpeedY(float speed) {
	speedY = speed;
}
float Orange::getRotation() {
	return rotation;
}
float Orange::getOrientation() {
	return atan2(speedY, speedX) * 180/PI + 90;
}
void Orange::setRotation(float angle) {
	rotation = angle;
}
void Orange::randomizeSpeedX() {
	speedX = ((rand() % 200 - 100)*0.0002);
}
void Orange::randomizeSpeedY() {
	speedY = ((rand() % 200 - 100)*0.0002);
}

void Orange::update(float time) {
	float posX = orangeX + speedX * time;
	float posY = orangeY + speedY * time;
	float disX = posX - orangeX;
	float disY = posY - orangeY;
	orangeX += disX;// * cos(rotation);
	orangeY += disY;// * sin(rotation);
	if (orangeX > 60.0)  { orangeX = -55.0; speedY = ((rand() % 200 - 100)*0.0002) + time / 10000; }
	if (orangeX < -60.0) { orangeX = 55.0;  speedY = ((rand() % 200 - 100)*0.0002) + time / 10000; }
	if (orangeY > 60.0)  { orangeY = -55.0; speedX = ((rand() % 200 - 100)*0.0002) + time / 10000; }
	if (orangeY < -60.0) { orangeY = 55;    speedX = ((rand() % 200 - 100)*0.0002) + time / 10000; }

	rotation += 0.1 * 180 / PI;
		if (rotation > 360)
			rotation -= 360;
}

void Orange::randomPos() {
	orangeX = orangeX * (rand() % 50);
	orangeX = orangeY * (rand() % 50);
}
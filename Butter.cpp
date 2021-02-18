#include"Butter.h"
#include<cmath>
#include<math.h>

#define PI 3.14159265358979323846

Butter::Butter(float x, float y, float z, std::string _name) {
	butterX = x;
	butterY = y;
	butterZ = z;
	name = _name;
}
float Butter::getButterX() {
	return butterX;
}
float Butter::getButterY() {
	return butterY;
}
float Butter::getButterZ() {
	return butterZ;
}
float Butter::getSpeed() {
	return speed;
}
void Butter::setSpeed(float s) {
	speed = s;
}
float Butter::getAcc() {
	return acceleration;
}
void Butter::setAcc(float a) {
	acceleration = a;
}
float Butter::getRotation() {
	return rotation;
}
void Butter::setRotation(float rotation_arg) {
	rotation = rotation_arg;
}
float Butter::getCollisionRadius() {
	return COLRADIUSB;
}
void Butter::setRadius(float radius_arg){
	radius = radius_arg;
}
void Butter::update(float time) {
	float posX = butterX + speed * time + 0.5 * acceleration * time * time;
	float posY = butterY + speed * time + 0.5 * acceleration * time * time;
	float disX = posX - butterX;
	float disY = posY - butterY;
	butterX += disX * cos(rotation);
	butterY += disY * sin(rotation);
}
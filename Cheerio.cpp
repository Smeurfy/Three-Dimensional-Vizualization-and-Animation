#include"Cheerio.h"
#include<cmath>
#include<math.h>

#define PI 3.14159265358979323846

Cheerio::Cheerio(float x, float y, float z, std::string _name) {
	cheerioX = x;
	cheerioY = y;
	cheerioZ = z;
	name = _name;
}
float Cheerio::getCheerioX() {
	return cheerioX;
}
float Cheerio::getCheerioY() {
	return cheerioY;
}
float Cheerio::getCheerioZ() {
	return cheerioZ;
}
float Cheerio::getAcc() {
	return acceleration;
}
void Cheerio::setAcc(float a) {
	acceleration = a;
}
float Cheerio::getSpeed() {
	return speed;
}
void Cheerio::setSpeed(float s) {
	speed = s;
}
float Cheerio::getRotation() {
	return rotation;
}
void Cheerio::setRotation(float rotation_arg) {
	rotation = rotation_arg;
}
float Cheerio::getCollisionRadius() {
	return COLRADIUSCH;
}
void Cheerio::setRadius(float radius_arg){
	radius = radius_arg;
}
void Cheerio::update(float time) {
	float posX = cheerioX + speed * time + 0.5 * acceleration * time * time;
	float posY = cheerioY + speed * time + 0.5 * acceleration * time * time;
	float disX = posX - cheerioX;
	float disY = posY - cheerioY;
	cheerioX += disX * cos(rotation);
	cheerioY += disY * sin(rotation);
}
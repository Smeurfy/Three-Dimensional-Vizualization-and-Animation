#include <iostream>

const double COLRADIUSCH = 0.7;

class Cheerio{

private:
	std::string name;
	float cheerioX;
	float cheerioY;
	float cheerioZ;
	float radius;
	float rotation;
	float speed;
	float acceleration;
	
public:
	Cheerio(float x, float y,float z, std::string name);
	float getCheerioX();
	float getCheerioY();
	float getCheerioZ();
	float getRotation();
	void setRotation(float rotation_arg);
	float getCollisionRadius();
	float getSpeed();
	void setSpeed(float speed);
	float getAcc();
	void setAcc(float a);
	void setRadius(float radius_arg);
	void update(float time);
};

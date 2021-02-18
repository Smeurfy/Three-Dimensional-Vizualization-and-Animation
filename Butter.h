#include <iostream>

const double COLRADIUSB = 2;

class Butter{

private:
	std::string name;
	float butterX;
	float butterY;
	float butterZ;
	float radius;
	float rotation;
	float speed;
	float acceleration;


public:
	Butter(float x, float y,float z, std::string name);
	float getButterX();
	float getButterY();
	float getButterZ();
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

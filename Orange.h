#include <iostream>

const double COLRADIUSO = 2;

class Orange{

private:
	std::string name;
	float orangeX;
	float orangeY;
	float orangeZ;
	float speedX = 0.01f;
	float speedY = -0.02;
	float rotation = 5;
	
public:
	Orange(float x, float y,float z, std::string name);
	float getOrangeX();
	float getOrangeY();
	float getOrangeZ();
	float getSpeedX();
	float getSpeedY();
	float getRotation();
	float getOrientation();
	float getCollisionRadius();
	void setSpeedX(float speed);
	void setSpeedY(float speed);
	void setRotation(float angle);
	void randomizeSpeedX();
	void randomizeSpeedY();
	void update(float time);
	void randomPos();
};

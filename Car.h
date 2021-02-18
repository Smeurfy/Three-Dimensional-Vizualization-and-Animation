#include <iostream>


const double COLRADIUSC = 1.5;

class Car{

private:
	std::string name;
	float carX;
	float carY;
	float carZ;
	int lives = 5;
	unsigned long score = 0;
	float maxSpeedFront = 0.01f;
	float maxSpeedBack = -0.005f;
	float maxAccFront = 0.001f;
	float maxAccBack = -0.001f;
	float speed = 0;
	float acceleration = 0;
	double rotation = 0;
	float handBrake = -0.01f;
	float movFrictionFront = -0.00001f;
	float movFrictionBack = 0.000002f;

	bool hasCollisionF = false;
	bool hasCollisionB = false;
	
	bool moveForward = false;
	bool moveLeft = false;
	bool moveRight = false;
	bool moveBack = false;
	bool forwardFriction = false;
	bool backFriction = false;
public:
	Car(float x, float y,float z, std::string name);
	float getCarX();
	void setCarX(float x);
	float getCarY();
	void setCarY(float y);
	float getCarZ();
	void setCarZ(float z);
	float getSpeed();
	void setSpeed(float s);
	float getAccFront();
	float getCollisionRadius();
	float getAccBack();
	float getAcceleration();
	void setAcceleration(float acc);
	bool getMoveForward();
	void setMoveForward(bool forward);
	bool getMoveBack();
	void setMoveBack(bool back);
	bool getMoveRight();
	void setMoveRight(bool right);
	bool getMoveLeft();
	void setMoveLeft(bool left);
	float getRotation();
	void setRotation(float angle);
	bool getForwardFriction();
	void setForwardFriction(bool forwardFriction);
	bool getBackFriction();
	void setBackFriction(bool backFriction);
	float getMovFrictionFront();
	float getMovFrictionBack();
	bool getHasCollisionF();
	void setHasCollisionF(bool col);
	bool getHasCollisionB();
	void setHasCollisionB(bool col);
	void update(float time);
	int getLives();
	void decLives();
	unsigned long getScore();
	void setScore(int value);
};

//
// AVT demo light 
// based on demos from GLSL Core Tutorial in Lighthouse3D.com   
//
// This demo was built for learning purposes only.
// Some code could be severely optimised, but I tried to
// keep as simple and clear as possible.
//
// The code comes with no warranties, use it at your own risk.
// You may use it, or parts of it, wherever you want.
//

#include <math.h>
#include <iostream>
#include <sstream>
#include <string>

// include GLEW to access OpenGL 3.3 functions
#include <GL/glew.h>


// GLUT is the toolkit to interface with the OS
#include <GL/freeglut.h>

// Use Very Simple Libs
#include "VSShaderlib.h"
#include "AVTmathLib.h"
#include "VertexAttrDef.h"
#include "basic_geometry.h"
#include "TGA.h"
#include "Car.h"
#include "Orange.h"
#include "Butter.h"
#include "Cheerio.h"
#include "l3dBillboard.h"

#define CAPTION "AVT Per Fragment Phong Lightning Demo"

//FIREWORKS
#define frand()			((float)rand()/RAND_MAX)
#define M_PI			3.14159265
#define MAX_PARTICULAS  500


int WindowHandle = 0;
int WinX = 800, WinY = 600;

unsigned int FrameCount = 0;


VSShaderLib shader;

struct MyMesh mesh[10];
int objId = 0; //id of the object mesh - to be used as index of mesh: mesh[objID] means the current mesh


//External array storage defined in AVTmathLib.cpp

/// The storage for matrices
extern float mMatrix[COUNT_MATRICES][16];
extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];

/// The normal matrix
extern float mNormal3x3[9];

GLint pvm_uniformId;
GLint vm_uniformId;
GLint normal_uniformId;
GLint lPos_uniformId;

GLint tex_loc, tex_loc1, tex_loc2, tex_loc3, tex_loc4;
GLint texMode_uniformId;

GLuint TextureArray[4];

// Camera Position
float camX, camY, camZ;

// Mouse Tracking Variables
int startX, startY, tracking = 0;

// Camera Spherical Coordinates
float alpha = 39.0f, beta = 51.0f;
float r = 10.0f;

// Frame counting and FPS computation
long myTime, timebase = 0, frame = 0;
char s[32];
float lightPos[4] = { 0.0f, 0.0f, -10.0f, 0.0f };

//cameras
int cameraID = 3;// starts with ortho camera
int orthoID = 1, perspec = 2, carPerspec = 3;

float timeSinceStart;
float oldTimeSinceStart;
#define PI 3.14159265358979323846

Car* _car;
std::vector<Orange*> _oranges;
std::vector<Butter*> _butters;
std::vector<Cheerio*> _cheerios;

// point lights
const int POINTLIGHTS_NR = 6;
const int SPOTLIGHTS_NR = 2;
typedef struct {
	float pos[4];
	float ambient[4];
	float diffuse[4];
	float specular[4];
	bool isEnabled;
	bool isLocal;
	bool isSpot;
	float linearAtt = 0.2;
	float constAtt = 0.4;
	float quadAtt = 0;
	float cutOff;
	float exponent;
	float cone;
} PointLight;

std::vector<PointLight> pointLights;
bool dir_state = true;

//FIREWORKS
typedef struct {
	float	life;		// vida
	float	fade;		// fade
	float	r, g, b;    // color
	GLfloat x, y, z;    // posicao
	GLfloat vx, vy, vz; // velocidade 
	GLfloat ax, ay, az; // aceleracao
} Particle;
Particle particles[MAX_PARTICULAS];
int dead_num_particles = 0;
int fireworks = 0;


// is the game paused?
bool pause = false;

// is D. Sebastiao incoming?
bool fog = false;
bool test = false;

void restartGame(); //function signature in order to compile
//FIREWORKS
void setParticles();
void iterateParticles(int value);


void timer(int value)
{
	std::ostringstream oss;
	oss << CAPTION << ": " << FrameCount << " FPS @ (" << WinX << "x" << WinY << ")";
	std::string s = oss.str();
	glutSetWindow(WindowHandle);
	glutSetWindowTitle(s.c_str());
	FrameCount = 0;
	glutTimerFunc(1000, timer, 0);
}

void refresh(int value)
{
	glutPostRedisplay();
	glutTimerFunc(1000 / 60, refresh, 0);
}

// ------------------------------------------------------------
//
// Reshape Callback Function
//

void changeSize(int w, int h) {

	//float ratio;
	//// Prevent a divide by zero, when window is too short
	//if (h == 0)
	//	h = 1;
	//// set the viewport to be the entire window
	//glViewport(0, 0, w, h);
	//// set the projection matrix
	//ratio = (1.0f * w) / h;
	//loadIdentity(PROJECTION);
	//perspective(53.13f, ratio, 0.1f, 1000.0f);


}


// ------------------------------------------------------------
//
// Render stufff
//

void setUniformLocations(int objId) {
	GLint loc;
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
	glUniform4fv(loc, 1, mesh[objId].mat.ambient);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
	glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
	glUniform4fv(loc, 1, mesh[objId].mat.specular);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
	glUniform1f(loc, mesh[objId].mat.shininess);
}

void sendMatricesAndRenderMesh(int objId) {
	// send matrices to OGL
	computeDerivedMatrix(PROJ_VIEW_MODEL);
	glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
	glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
	computeNormalMatrix3x3();
	glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

	// Render mesh
	glBindVertexArray(mesh[objId].vao);
}

void renderTable() {
	objId = 0;

	// send the material
	setUniformLocations(objId);

	pushMatrix(MODEL);
	translate(MODEL, -50.0f, -50.0f, 0.0f);
	scale(MODEL, 100.0f, 100.0f, 0.5f);

	// send matrices to OGL and render Mesh
	sendMatricesAndRenderMesh(objId);

	if (!shader.isProgramValid()) {
		printf("Program Not Valid!\n");
		exit(1);
	}
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	popMatrix(MODEL);
}

void renderCar() {
	objId = 1;

	// send the material
	setUniformLocations(objId);

	pushMatrix(MODEL);
	translate(MODEL, _car->getCarX(), _car->getCarY(), 1.0f);
	rotate(MODEL, _car->getRotation() * 180 / PI, 0, 0, 1);
	translate(MODEL, -1.5f, -1.0f, 0.5f);//coloca eixo de rotaçao no meio
	scale(MODEL, 3.0f, 2.0f, 1.5f);

	//FIREWORKS
	float car_x = _car->getCarX();
	float car_y = _car->getCarY();
	if (car_x > -5 && car_x < 0 && car_y < 0 && fireworks == 0) {
		fireworks = 1;
		setParticles();
		glutTimerFunc(0, iterateParticles, 0);  //timer for particle system
	}

	// send matrices to OGL and render Mesh
	sendMatricesAndRenderMesh(objId);

	if (!shader.isProgramValid()) {
		printf("Program Not Valid!\n");
		exit(1);
	}
	glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	popMatrix(MODEL);
}

void renderWheel() {
	objId = 2;
	float wheel_p[] = { 1.0f, 1.0f, -1.0f, -1.0f,
						-1.0f, 1.0f, 1.0f, -1.0f };

	for (int i = 0; i < 4; i++) {
		// send the material
		setUniformLocations(objId);

		pushMatrix(MODEL);
		translate(MODEL, _car->getCarX() + wheel_p[i] * cos(_car->getRotation()) - wheel_p[i + 4] * sin(_car->getRotation()),
			_car->getCarY() + wheel_p[i + 4] * cos(_car->getRotation()) + wheel_p[i] * sin(_car->getRotation()), 1.5f);
		rotate(MODEL, _car->getRotation() * 180 / PI, 0, 0, 1);

		// send matrices to OGL and render Mesh
		sendMatricesAndRenderMesh(objId);

		if (!shader.isProgramValid()) {
			printf("Program Not Valid!\n");
			exit(1);
		}
		glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		popMatrix(MODEL);
	}
}

void renderCheerio() {
	objId = 3;

	for (int i = 0; i < 266; i++) {
		// send the material
		setUniformLocations(objId);

		pushMatrix(MODEL);
		translate(MODEL, _cheerios.at(i)->getCheerioX(), _cheerios.at(i)->getCheerioY(), _cheerios.at(i)->getCheerioZ());
		rotate(MODEL, 90, 1, 0, 0);
		// send matrices to OGL and render Mesh
		sendMatricesAndRenderMesh(objId);

		if (!shader.isProgramValid()) {
			printf("Program Not Valid!\n");
			exit(1);
		}
		
		glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		popMatrix(MODEL);
	}
}

void renderButter() {
	objId = 5;

	for (int i = 0; i < 4; i++) {
		// send the material
		setUniformLocations(objId);

		pushMatrix(MODEL);
		translate(MODEL, _butters.at(i)->getButterX(), _butters.at(i)->getButterY(), _butters.at(i)->getButterZ());
		translate(MODEL, -2.0f, -1.0f, 0.0f);//coloca eixo de rotaçao no meio
		scale(MODEL, 4.0f, 2.0f, 2.0f);
		// send matrices to OGL and render Mesh
		sendMatricesAndRenderMesh(objId);

		if (!shader.isProgramValid()) {
			printf("Program Not Valid!\n");
			exit(1);
		}
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		popMatrix(MODEL);
	}
}

void renderOrange() {
	objId = 6;
	for (int i = 0; i < 5; i++) {
		// send the material
		setUniformLocations(objId);

		pushMatrix(MODEL);
		translate(MODEL, _oranges.at(i)->getOrangeX(), _oranges.at(i)->getOrangeY(), _oranges.at(i)->getOrangeZ());
		rotate(MODEL, _oranges.at(i)->getOrientation(), 0, 0, 1);
		rotate(MODEL, _oranges.at(i)->getRotation(), 1, 0, 0);
		// send matrices to OGL and render Mesh
		sendMatricesAndRenderMesh(objId);

		if (!shader.isProgramValid()) {
			printf("Program Not Valid!\n");
			exit(1);
		}

		glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		popMatrix(MODEL);
	}
	
}

void renderLights() {
	GLint loc;

	// render lights
	float res[4];
	float aux[4];
	std::string to_shader = "";
	for (int i = 0; i < POINTLIGHTS_NR; i++) {
		aux[0] = pointLights.at(i).pos[0];
		aux[1] = pointLights.at(i).pos[1];
		aux[2] = pointLights.at(i).pos[2];
		aux[3] = pointLights.at(i).pos[3];
		multMatrixPoint(VIEW, aux, res);
		to_shader = "lights[" + std::to_string(i + 1) + "].position";
		loc = glGetUniformLocation(shader.getProgramIndex(), to_shader.c_str());
		glUniform4fv(loc, 1, res);
		aux[0] = pointLights.at(i).ambient[0];
		aux[1] = pointLights.at(i).ambient[1];
		aux[2] = pointLights.at(i).ambient[2];
		aux[3] = pointLights.at(i).ambient[3];
		to_shader = "lights[" + std::to_string(i + 1) + "].ambient";
		loc = glGetUniformLocation(shader.getProgramIndex(), to_shader.c_str());
		glUniform4fv(loc, 1, aux);
		aux[0] = pointLights.at(i).diffuse[0];
		aux[1] = pointLights.at(i).diffuse[1];
		aux[2] = pointLights.at(i).diffuse[2];
		aux[3] = pointLights.at(i).diffuse[3];
		to_shader = "lights[" + std::to_string(i + 1) + "].diffuse";
		loc = glGetUniformLocation(shader.getProgramIndex(), to_shader.c_str());
		glUniform4fv(loc, 1, aux);
		aux[0] = pointLights.at(i).specular[0];
		aux[1] = pointLights.at(i).specular[1];
		aux[2] = pointLights.at(i).specular[2];
		aux[3] = pointLights.at(i).specular[3];
		to_shader = "lights[" + std::to_string(i + 1) + "].specular";
		loc = glGetUniformLocation(shader.getProgramIndex(), to_shader.c_str());
		glUniform4fv(loc, 1, aux);
		to_shader = "lights[" + std::to_string(i + 1) + "].isEnabled";
		loc = glGetUniformLocation(shader.getProgramIndex(), to_shader.c_str());
		glUniform1i(loc, pointLights.at(i).isEnabled);
		/*to_shader = "lights[" + std::to_string(i + 1) + "].isLocal";
		loc = glGetUniformLocation(shader.getProgramIndex(), to_shader.c_str());
		glUniform1i(loc, pointLights.at(i).isLocal);*/
		to_shader = "lights[" + std::to_string(i + 1) + "].isSpot";
		loc = glGetUniformLocation(shader.getProgramIndex(), to_shader.c_str());
		glUniform1i(loc, pointLights.at(i).isSpot);
		to_shader = "lights[" + std::to_string(i + 1) + "].linearAttenuation";
		loc = glGetUniformLocation(shader.getProgramIndex(), to_shader.c_str());
		glUniform1i(loc, pointLights.at(i).linearAtt);
		to_shader = "lights[" + std::to_string(i + 1) + "].constantAttenuation";
		loc = glGetUniformLocation(shader.getProgramIndex(), to_shader.c_str());
		glUniform1i(loc, pointLights.at(i).constAtt);
		to_shader = "lights[" + std::to_string(i + 1) + "].quadraticAttenuation";
		loc = glGetUniformLocation(shader.getProgramIndex(), to_shader.c_str());
		glUniform1i(loc, pointLights.at(i).quadAtt);
	}

	//left car light
	aux[0] = _car->getCarX() + 1.5 * cos(_car->getRotation()) + 0.5 * sin(_car->getRotation());
	aux[1] = _car->getCarY() - 1.5 * cos(_car->getRotation()) + 0.5 * sin(_car->getRotation());
	aux[2] = 1.0f;
	aux[3] = 1.0f;
	multMatrixPoint(VIEW, aux, res);
	to_shader = "lights[7].position";
	loc = glGetUniformLocation(shader.getProgramIndex(), to_shader.c_str());
	glUniform4fv(loc, 1, res);

	//right car light
	aux[0] = _car->getCarX() + 1.5 * cos(_car->getRotation()) - 0.5 * sin(_car->getRotation());
	aux[1] = _car->getCarY() + 1.5 * cos(_car->getRotation()) + 0.5 * sin(_car->getRotation());
	aux[2] = 1.0f;
	aux[3] = 1.0f;
	multMatrixPoint(VIEW, aux, res);
	to_shader = "lights[8].position";
	loc = glGetUniformLocation(shader.getProgramIndex(), to_shader.c_str());
	glUniform4fv(loc, 1, res);
	for (int i = 6; i < 6 + SPOTLIGHTS_NR; i++) {
		aux[0] = cos(_car->getRotation());
		aux[1] = sin(_car->getRotation());
		aux[2] = 0.0f;
		aux[3] = 0.0f;
		multMatrixPoint(VIEW, aux, res);
		to_shader = "lights[" + std::to_string(i + 1) + "].direction";
		loc = glGetUniformLocation(shader.getProgramIndex(), to_shader.c_str());
		glUniform4fv(loc, 1, res);
		aux[0] = pointLights.at(i).ambient[0];
		aux[1] = pointLights.at(i).ambient[1];
		aux[2] = pointLights.at(i).ambient[2];
		aux[3] = pointLights.at(i).ambient[3];
		to_shader = "lights[" + std::to_string(i + 1) + "].ambient";
		loc = glGetUniformLocation(shader.getProgramIndex(), to_shader.c_str());
		glUniform4fv(loc, 1, aux);
		aux[0] = pointLights.at(i).diffuse[0];
		aux[1] = pointLights.at(i).diffuse[1];
		aux[2] = pointLights.at(i).diffuse[2];
		aux[3] = pointLights.at(i).diffuse[3];
		to_shader = "lights[" + std::to_string(i + 1) + "].diffuse";
		loc = glGetUniformLocation(shader.getProgramIndex(), to_shader.c_str());
		glUniform4fv(loc, 1, aux);
		aux[0] = pointLights.at(i).specular[0];
		aux[1] = pointLights.at(i).specular[1];
		aux[2] = pointLights.at(i).specular[2];
		aux[3] = pointLights.at(i).specular[3];
		to_shader = "lights[" + std::to_string(i + 1) + "].specular";
		loc = glGetUniformLocation(shader.getProgramIndex(), to_shader.c_str());
		glUniform4fv(loc, 1, aux);
		to_shader = "lights[" + std::to_string(i + 1) + "].isEnabled";
		loc = glGetUniformLocation(shader.getProgramIndex(), to_shader.c_str());
		glUniform1i(loc, pointLights.at(i).isEnabled);
		to_shader = "lights[" + std::to_string(i + 1) + "].isLocal";
		loc = glGetUniformLocation(shader.getProgramIndex(), to_shader.c_str());
		glUniform1i(loc, pointLights.at(i).isLocal);
		to_shader = "lights[" + std::to_string(i + 1) + "].isSpot";
		loc = glGetUniformLocation(shader.getProgramIndex(), to_shader.c_str());
		glUniform1i(loc, pointLights.at(i).isSpot);
		to_shader = "lights[" + std::to_string(i + 1) + "].linearAttenuation";
		loc = glGetUniformLocation(shader.getProgramIndex(), to_shader.c_str());
		glUniform1i(loc, pointLights.at(i).linearAtt);
		to_shader = "lights[" + std::to_string(i + 1) + "].constantAttenuation";
		loc = glGetUniformLocation(shader.getProgramIndex(), to_shader.c_str());
		glUniform1i(loc, pointLights.at(i).constAtt);
		to_shader = "lights[" + std::to_string(i + 1) + "].quadraticAttenuation";
		loc = glGetUniformLocation(shader.getProgramIndex(), to_shader.c_str());
		glUniform1i(loc, pointLights.at(i).quadAtt);
		to_shader = "lights[" + std::to_string(i + 1) + "].spotCosCutoff";
		loc = glGetUniformLocation(shader.getProgramIndex(), to_shader.c_str());
		glUniform1i(loc, pointLights.at(i).cutOff);
		to_shader = "lights[" + std::to_string(i + 1) + "].spotExponent";
		loc = glGetUniformLocation(shader.getProgramIndex(), to_shader.c_str());
		glUniform1i(loc, pointLights.at(i).exponent);
		to_shader = "lights[" + std::to_string(i + 1) + "].coneDirection";
		loc = glGetUniformLocation(shader.getProgramIndex(), to_shader.c_str());
		glUniform1i(loc, pointLights.at(i).cone);

	}
}

//FIREWORKS
void renderParticles() {
	// draw fireworks particles
	objId = 9;  //quad for particle
	float particle_color[4];
	float modelview[16];

	//glBindTexture(GL_TEXTURE_2D, TextureArray[1]); //particle.bmp associated to TU0 

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDepthMask(GL_FALSE);  //Depth Buffer Read Only

	glUniform1i(texMode_uniformId, 4); // draw modulated textured particles 

	for (int i = 0; i < MAX_PARTICULAS; i++)
	{
		if (particles[i].life > 0.0f) /* só desenha as que ainda estão vivas */
		{

			/* A vida da partícula representa o canal alpha da cor. Como o blend está activo a cor final é a soma da cor rgb do fragmento multiplicada pelo
			alpha com a cor do pixel destino */

			particle_color[0] = particles[i].r;
			particle_color[1] = particles[i].g;
			particle_color[2] = particles[i].b;
			particle_color[3] = particles[i].life;

			// send the material - diffuse color modulated with texture
			GLint loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
			glUniform4fv(loc, 1, particle_color);

			pushMatrix(MODEL);
			translate(MODEL, particles[i].x, particles[i].y, particles[i].z);
			if (cameraID == 1) {

			}
			if (cameraID == 2) {
				rotate(MODEL, 90, 0, 1, 0);
			}
			if (cameraID == 3) {
				rotate(MODEL, -90, 0, 1, 0);
			}

			// send matrices to OGL
			computeDerivedMatrix(PROJ_VIEW_MODEL);
			glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
			glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
			computeNormalMatrix3x3();
			glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

			glBindVertexArray(mesh[objId].vao);
			glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
			popMatrix(MODEL);
		}
		else dead_num_particles++;
	}

	

	glDepthMask(GL_TRUE); //make depth buffer again writeable

	if (dead_num_particles == MAX_PARTICULAS) {
		fireworks = 0;
		dead_num_particles = 0;
		printf("All particles dead\n");
	}
}

void moveObjects() {
	timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	float deltaTime = timeSinceStart - oldTimeSinceStart;
	oldTimeSinceStart = timeSinceStart;
	if (pause || _car->getLives() == 0) deltaTime = 0;

	_car->update(deltaTime);
	/*for (int i = 0; i < _vec.size(); i++) {
		_vec.at(i)->update(deltaTime);
	}*/
	for (int i = 0; i < _oranges.size(); i++) {
		_oranges.at(i)->update(deltaTime);
	}
	//collision with endTabel
	if (_car->getCarX() > 55 || _car->getCarX() < -55 || _car->getCarY() > 55 || _car->getCarY() < -55) {
		_car->setCarX(0);
		_car->setCarY(-12);
		_car->setCarZ(0);
		_car->setSpeed(0);
		_car->setAcceleration(0);
		_car->setRotation(0);
		_car->setScore(-10);
	}

	//Collision with oranges
	for (int i = 0; i < _oranges.size(); i++) {
		if (sqrt(pow(_oranges[i]->getOrangeX() - _car->getCarX(), 2) + pow(_oranges[i]->getOrangeY() - _car->getCarY(), 2)) < _oranges[i]->getCollisionRadius() + _car->getCollisionRadius()) {
			_car->setCarX(0);
			_car->setCarY(-12);
			_car->setCarZ(0);
			_car->setSpeed(0);
			_car->setAcceleration(0);
			_car->setRotation(0);
			_car->decLives();
			_car->setScore(-100);
			_oranges[i]->randomPos();
		}
	}
	//Collision with butters
	for (int i = 0; i < _butters.size(); i++) {
		if (sqrt(pow(_butters[i]->getButterX() - _car->getCarX(), 2) + pow(_butters[i]->getButterY() - _car->getCarY(), 2)) < _butters[i]->getCollisionRadius() + _car->getCollisionRadius()) {
			_car->setScore(-50);
			if (_car->getSpeed() > 0) {
				_butters[i]->setSpeed(_car->getSpeed());
				_butters[i]->setAcc(0.009f);
				_butters[i]->setRotation(_car->getRotation());
				_butters[i]->update(deltaTime);
				_car->setHasCollisionF(true);
				_car->setAcceleration(0);
				_car->setSpeed(0.0f);
			}
			else {
				_butters[i]->setSpeed(_car->getSpeed());
				_butters[i]->setAcc(-0.009f);
				_butters[i]->setRotation(_car->getRotation());
				_butters[i]->update(deltaTime);
				_car->setHasCollisionB(true);
				_car->setAcceleration(0);
				_car->setSpeed(0.0f);
			}
		}
	}
	//Collision with cheerios
	for (int i = 0; i < _cheerios.size(); i++) {
		if (sqrt(pow(_cheerios[i]->getCheerioX() - _car->getCarX(), 2) + pow(_cheerios[i]->getCheerioY() - _car->getCarY(), 2)) < _cheerios[i]->getCollisionRadius() + _car->getCollisionRadius()) {
			_car->setScore(-20);
			if (_car->getSpeed() > 0) {
				_cheerios[i]->setSpeed(_car->getSpeed());
				_cheerios[i]->setAcc(0.009f);
				_cheerios[i]->setRotation(_car->getRotation());
				_cheerios[i]->update(deltaTime);
				_car->setHasCollisionF(true);
				_car->setAcceleration(0);
				_car->setSpeed(0.0f);
			}
			else {
				_cheerios[i]->setSpeed(_car->getSpeed());
				_cheerios[i]->setAcc(-0.009f);
				_cheerios[i]->setRotation(_car->getRotation());
				_cheerios[i]->update(deltaTime);
				_car->setHasCollisionB(true);
				_car->setAcceleration(0);
				_car->setSpeed(0.0f);
			}
		}
	}
}

void updateGame() {
	std::ostringstream oss;
	if (pause && _car->getLives() > 0) {
		oss << CAPTION << " | GAME PAUSED: press S to continue";
	}
	else if (_car->getLives() == 0) {
		pause = true;
		oss << CAPTION << " | GAME OVER: press R to restart game";
	}
	else {
		oss << CAPTION << " | Lives: " << _car->getLives() << " | Points: " << _car->getScore();
	}
	// put the caption on window title
	std::string output = oss.str();
	glutSetWindow(WindowHandle);
	glutSetWindowTitle(output.c_str());
}

void renderBillBoards()
{
	GLint loc;
	float modelview[16];
	float pos_x[] = { 5,10,15,22,22,22,22,22,15,10
					 ,5,0,-5,-10,-15,-22,-22,-22,-22,-22,
					 -22,-22,-22,-22,-15,-10,-5,0 };
	float pos_y[] = { 0,0,0,0,5,10,15,22,22,22,
					  22,22,22,22,22,22,22,15,10,5,
					  0,-5,-10,-15,-10,-5,0,0 };
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (int i = 0; i < 28; i++) {
		glUniform1i(texMode_uniformId, 3); // draw textured quads
		pushMatrix(MODEL);
		loadIdentity(MODEL);
		translate(MODEL, pos_x[i], pos_y[i], 3);
		rotate(MODEL, 90, 1, 0, 0);

		computeDerivedMatrix(VIEW_MODEL);
		memcpy(modelview, mCompMatrix[VIEW_MODEL], sizeof(float) * 16);  //save VIEW_MODEL in modelview matrix

		BillboardCheatSphericalBegin();

		computeDerivedMatrix_PVM(); // calculate PROJ_VIEW_MODEL

		objId = 8;
		//diffuse and ambient color are not used in the tree quads
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
		glUniform4fv(loc, 1, mesh[objId].mat.specular);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
		glUniform1f(loc, mesh[objId].mat.shininess);

		glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
		glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
		computeNormalMatrix3x3();
		glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

		glBindVertexArray(mesh[objId].vao);
		glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
		popMatrix(MODEL);
	}

}

void circleForStencilArea() {
	objId = 7;

	glClear(GL_STENCIL_BUFFER_BIT);

	glStencilFunc(GL_NEVER, 0x1, 0x1);
	glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);
	glUniform1i(texMode_uniformId, 0);

	// send the material
	//nao vai ser desenhado
	//setUniformLocations(objId);
	loadIdentity(PROJECTION);
	loadIdentity(VIEW);
	loadIdentity(MODEL);

	perspective(100.0f, 1.0f, 0.1f, 100.0f);
	lookAt(_car->getCarX() + 5 * cos(_car->getRotation()), _car->getCarY() + 5 * sin(_car->getRotation()), 7,
		_car->getCarX(), _car->getCarY(), 3.45,
		0, 0, 1);
	pushMatrix(MODEL);
	translate(MODEL, _car->getCarX(), _car->getCarY(), 5);
	float modelview[16];
	computeDerivedMatrix(VIEW_MODEL);
	memcpy(modelview, mCompMatrix[VIEW_MODEL], sizeof(float) * 16);  //save VIEW_MODEL in modelview matrix

	BillboardCheatSphericalBegin();

	computeDerivedMatrix_PVM(); // calculate PROJ_VIEW_MODEL

	// send matrices to OGL and render Mesh
	glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
	glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
	computeNormalMatrix3x3();
	glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

	glBindVertexArray(mesh[objId].vao);

	glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	popMatrix(MODEL);

	glStencilFunc(GL_EQUAL, 0x1, 0x1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	loadIdentity(MODEL);
	moveObjects();
	renderLights();
	glUniform1i(texMode_uniformId, 2); // render table with lightwood.tga
	renderTable();
	glUniform1i(texMode_uniformId, 4); // render materials without textures, only material
	renderCar();
	renderWheel();
	renderCheerio();
	renderButter();
	renderOrange();
	renderBillBoards();
}

void renderScene(void) {

	GLint loc;
	glEnable(GL_STENCIL_TEST);
	FrameCount++;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureArray[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, TextureArray[1]);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, TextureArray[2]);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, TextureArray[3]);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, TextureArray[4]);
	glUniform1i(tex_loc, 0);
	glUniform1i(tex_loc1, 1);
	glUniform1i(tex_loc2, 2);
	glUniform1i(tex_loc3, 3);
	glUniform1i(tex_loc4, 4);

	float res[4];
	float aux[4];
	float amb[] = { 0.2f, 0.15f, 0.1f, 1.0f };
	float diff[] = { 203.0 / 255.0f , 206.0 / 255.0f, 39.0 / 255.0f, 1.0f };
	float spec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	aux[0] = lightPos[0];
	aux[1] = lightPos[1];
	aux[2] = lightPos[2];
	aux[3] = lightPos[3];
	multMatrixPoint(VIEW, aux, res);
	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[0].position");
	glUniform4fv(loc, 1, res);
	aux[0] = amb[0];
	aux[1] = amb[1];
	aux[2] = amb[2];
	aux[3] = amb[3];
	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[0].ambient");
	glUniform4fv(loc, 1, aux);
	aux[0] = diff[0];
	aux[1] = diff[1];
	aux[2] = diff[2];
	aux[3] = diff[3];
	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[0].diffuse");
	glUniform4fv(loc, 1, aux);
	aux[0] = spec[0];
	aux[1] = spec[1];
	aux[2] = spec[2];
	aux[3] = spec[3];
	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[0].specular");
	glUniform4fv(loc, 1, aux);
	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[0].isEnabled");
	glUniform1i(loc, dir_state);
	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[0].isLocal");
	glUniform1i(loc, false);


	glUseProgram(shader.getProgramIndex());

	if (_car->getMoveBack() && cameraID == 3) {
		test = true;
		circleForStencilArea();
	}
	if (_car->getMoveBack() && cameraID == 2) {
		glDisable(GL_STENCIL_TEST);
	}
	if (_car->getMoveBack() && cameraID == 1) {
		glDisable(GL_STENCIL_TEST);
	}
	if (!_car->getMoveBack() && test) {
		glDisable(GL_STENCIL_TEST);
	}

	if (cameraID == 1) {
		loadIdentity(PROJECTION);
		ortho(-55.0f, 55.0f, -55.0f, 55.0f, -10, 10);
		loadIdentity(VIEW);
		loadIdentity(MODEL);
		lookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);
	}
	if (cameraID == 2) {
		loadIdentity(PROJECTION);
		perspective(75.0f, 1.0f, 0.1f, 1000.0f);
		loadIdentity(VIEW);
		loadIdentity(MODEL);
		lookAt(50, 50, 50,
			0, 0, 2,
			0, 0, 1);
	}
	if (cameraID == 3) {
		loadIdentity(PROJECTION);
		perspective(75.0f, 1.0f, 0.1f, 1000.0f);
		loadIdentity(VIEW);
		loadIdentity(MODEL);
		lookAt(_car->getCarX() - 10 * cos(_car->getRotation()), _car->getCarY() - 10 * sin(_car->getRotation()), _car->getCarZ() + 5,
			_car->getCarX(), _car->getCarY(), _car->getCarZ(),
			0, 0, 1);
	}
	loc = glGetUniformLocation(shader.getProgramIndex(), "cameraID");
	glUniform1i(loc, cameraID);



	glStencilFunc(GL_NOTEQUAL, 0x1, 0x1);
	moveObjects();
	renderLights();
	glUniform1i(texMode_uniformId, 2); // render table with lightwood.tga
	renderTable();
	glUniform1i(texMode_uniformId, 4); // render materials without textures, only material
	renderCar();
	renderWheel();
	renderCheerio();
	renderButter();
	renderOrange();

	//Billboards
	renderBillBoards();

	//FIREWORKS
	if (fireworks) {
		glUniform1i(texMode_uniformId, 4); // render materials without textures, only material
		renderParticles();
	}

	glDisable(GL_BLEND);
	// fog effect
	loc = glGetUniformLocation(shader.getProgramIndex(), "isFog");
	glUniform1i(loc, fog);

	updateGame();

	glutSwapBuffers();
}

// ------------------------------------------------------------
//
// Events from the Keyboard
//
void keyUp(unsigned char key, int x, int y) {
	switch (key) {
	case 'q': // move forward
		_car->setMoveForward(false);
		if (_car->getSpeed() > 0) {
			_car->setForwardFriction(true);
			_car->setAcceleration(_car->getMovFrictionFront());
		}
		break;
	case 'a': // move backward
		_car->setMoveBack(false);
		if (_car->getSpeed() < 0) {
			_car->setBackFriction(true);
			_car->setAcceleration(_car->getMovFrictionBack());
		}
		if (_car->getMoveForward()) {

		}
		break;
	case 'p': // move right
		_car->setMoveRight(false);
		break;
	case 'o': // move left
		_car->setMoveLeft(false);
		break;
	}
}

void processKeys(unsigned char key, int xx, int yy)
{
	switch (key) {
	case '1':
		cameraID = 1;
		printf("Camera ortho\n");
		break;
	case '2':
		cameraID = 2;
		printf("Camera prespec\n");
		break;
	case '3':
		cameraID = 3;
		printf("Camera Carprespec\n");
		break;
	case 27:
		glutLeaveMainLoop();
		break;
	case 'q': // move forward
		_car->setHasCollisionB(false);
		if (!_car->getHasCollisionF()) {
			_car->setMoveForward(true);
			_car->setAcceleration(_car->getAccFront());
		}
		break;
	case 'a': // move backward
		_car->setHasCollisionF(false);
		if (!_car->getHasCollisionB()) {
			_car->setMoveBack(true);
			_car->setAcceleration(_car->getAccBack());
		}

		break;
	case 'p': // move right
		_car->setMoveRight(true);

		break;
	case 'o': // move left
		_car->setMoveLeft(true);
		break;
	case 'c':
		for (int i = 0; i < POINTLIGHTS_NR; i++)
			pointLights.at(i).isEnabled = !pointLights.at(i).isEnabled;
		break;
	case 'h':
		for (int i = 6; i < 6 + SPOTLIGHTS_NR; i++)
			pointLights.at(i).isEnabled = !pointLights.at(i).isEnabled;
		break;
	case 'n':
		dir_state = !dir_state;
		break;
	case 's':
		pause = !pause;
		break;
	case 'r':
		restartGame();
		break;
	case 'f':
		fog = !fog;
		break;
	case '0':
		fireworks = 1;
		setParticles();
		glutTimerFunc(0, iterateParticles, 0);  //timer for particle system
		break;
	}
}


// ------------------------------------------------------------
//
// Mouse Events
//

void processMouseButtons(int button, int state, int xx, int yy)
{
	// start tracking the mouse
	if (state == GLUT_DOWN) {
		startX = xx;
		startY = yy;
		if (button == GLUT_LEFT_BUTTON)
			tracking = 1;
		else if (button == GLUT_RIGHT_BUTTON)
			tracking = 2;
	}

	//stop tracking the mouse
	else if (state == GLUT_UP) {
		if (tracking == 1) {
			alpha -= (xx - startX);
			beta += (yy - startY);
		}
		else if (tracking == 2) {
			r += (yy - startY) * 0.01f;
			if (r < 0.1f)
				r = 0.1f;
		}
		tracking = 0;
	}
}

// Track mouse motion while buttons are pressed

void processMouseMotion(int xx, int yy)
{

	int deltaX, deltaY;
	float alphaAux, betaAux;
	float rAux;

	deltaX = -xx + startX;
	deltaY = yy - startY;

	// left mouse button: move camera
	if (tracking == 1) {


		alphaAux = alpha + deltaX;
		betaAux = beta + deltaY;

		if (betaAux > 85.0f)
			betaAux = 85.0f;
		else if (betaAux < -85.0f)
			betaAux = -85.0f;
		rAux = r;
	}
	// right mouse button: zoom
	else if (tracking == 2) {

		alphaAux = alpha;
		betaAux = beta;
		rAux = r + (deltaY * 0.01f);
		if (rAux < 0.1f)
			rAux = 0.1f;
	}

	camX = rAux * sin(alphaAux * 3.14f / 180.0f) * cos(betaAux * 3.14f / 180.0f);
	camZ = rAux * cos(alphaAux * 3.14f / 180.0f) * cos(betaAux * 3.14f / 180.0f);
	camY = rAux * sin(betaAux * 3.14f / 180.0f);

	//  uncomment this if not using an idle func
	//	glutPostRedisplay();
}


void mouseWheel(int wheel, int direction, int x, int y) {

	r += direction * 0.1f;
	if (r < 0.1f)
		r = 0.1f;

	camX = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camZ = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camY = r * sin(beta * 3.14f / 180.0f);

	//  uncomment this if not using an idle func
	//	glutPostRedisplay();
}

// --------------------------------------------------------
//
// Shader Stuff
//


GLuint setupShaders() {

	// Shader for models
	shader.init();

	shader.loadShader(VSShaderLib::VERTEX_SHADER, "shaders/pointlight.vert");
	shader.loadShader(VSShaderLib::FRAGMENT_SHADER, "shaders/pointlight.frag");


	// set semantics for the shader variables
	glBindFragDataLocation(shader.getProgramIndex(), 0, "colorOut");
	glBindAttribLocation(shader.getProgramIndex(), VERTEX_COORD_ATTRIB, "position");
	glBindAttribLocation(shader.getProgramIndex(), NORMAL_ATTRIB, "normal");
	glBindAttribLocation(shader.getProgramIndex(), TEXTURE_COORD_ATTRIB, "texCoord");

	glLinkProgram(shader.getProgramIndex());

	texMode_uniformId = glGetUniformLocation(shader.getProgramIndex(), "texMode"); // different modes of texturing
	pvm_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_pvm");
	vm_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_viewModel");
	normal_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_normal");
	lPos_uniformId = glGetUniformLocation(shader.getProgramIndex(), "l_pos");
	tex_loc = glGetUniformLocation(shader.getProgramIndex(), "texmap");
	tex_loc1 = glGetUniformLocation(shader.getProgramIndex(), "texmap1");
	tex_loc2 = glGetUniformLocation(shader.getProgramIndex(), "texmap2");
	tex_loc3 = glGetUniformLocation(shader.getProgramIndex(), "texmap3");
	tex_loc4 = glGetUniformLocation(shader.getProgramIndex(), "lensflare");

	printf("InfoLog for Per Fragment Phong Lightning Shader\n%s\n\n", shader.getAllInfoLogs().c_str());

	return(shader.isProgramLinked());
}


void meshSet(int objId, float amb[], float diff[], float spec[], float emissive[], float shininess, int texcount) {
	memcpy(mesh[objId].mat.ambient, amb, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, spec, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
}

void createTable() {
	float amb[] = { 0.2f, 0.15f, 0.1f, 1.0f };
	float diff[] = { 0.8f, 0.6f, 0.4f, 0.7f };
	float spec[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	float emissive[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininess = 100.0f;
	int texcount = 0;
	objId = 0;
	meshSet(objId, amb, diff, spec, emissive, shininess, texcount);
	createCube();
}

void createCar() {
	float amb[] = { 2.0f / 255.0f, 35.0f / 255.0f, 99.0f / 255.0f, 1.0f };
	float diff[] = { 66.0f / 255.0f, 134.0f / 255.0f, 244.0f / 255.0f, 1.0f };
	float spec[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	float emissive[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininess = 100.0f;
	int texcount = 0;
	objId = 1;
	meshSet(objId, amb, diff, spec, emissive, shininess, texcount);
	createCube();

	_car = new Car(0, -12, 0, "Car");

}
void createWheel() {
	float amb[] = { 0.2f, 0.15f, 0.1f, 1.0f };
	float diff[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float spec[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	float emissive[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float shininess = 50.0f;
	int texcount = 0;
	objId = 2;
	meshSet(objId, amb, diff, spec, emissive, shininess, texcount);
	createTorus(0.3f, 1.0f, 30, 30);
}
void createCheerio() {
	float cheerios_x[] = {  //OUTER RING
							-46.0, -44.0, -42.0, -40.0, -38.0, -36.0, -34.0, -32.0, -30.0, -28.0, -26.0, -24.0,
							-22.0, -20.0, -18.0, -16.0, -14.0, -12.0, -10.0,  -8.0,  -6.0,  -4.0,  -2.0,   0.0,
							  2.0,   4.0,   6.0,   8.0,  10.0,  12.0,  14.0,  16.0,  18.0,  20.0,  22.0,  24.0,
							 26.0,  28.0,  30.0,  32.0,  34.0,  36.0,  38.0,  40.0,  42.0,  44.0,  46.0,  46.0,

							 46.0,  46.0,  46.0,  46.0,  46.0,  46.0,  46.0,  46.0,  46.0,  46.0,  46.0,  46.0,
							 46.0,  46.0,  46.0,  46.0,  46.0,  46.0,  46.0,  46.0,  46.0,  46.0,  46.0,  46.0,
							 46.0,  46.0,  46.0,  46.0,  46.0,  46.0,  46.0,  46.0,  46.0,

							 46.0,  44.0,  42.0,  40.0,  38.0,  36.0,  34.0,  32.0,  30.0,  28.0,  26.0,  24.0,
							 22.0,  20.0,  18.0,  16.0,  14.0,  12.0,  10.0,   8.0,   6.0,   4.0,   2.0,   0.0,
							 -2.0,  -4.0,  -6.0,  -8.0, -10.0, -12.0, -14.0, -16.0, -18.0, -20.0, -22.0, -24.0,
							-26.0, -28.0, -30.0, -32.0, -34.0, -36.0, -38.0, -40.0, -42.0, -44.0, -46.0,

							-46.0, -46.0, -46.0, -46.0, -46.0, -46.0, -46.0, -46.0, -46.0, -46.0, -46.0, -46.0,
							-46.0, -46.0, -46.0, -46.0, -46.0, -46.0, -46.0, -46.0, -46.0, -46.0, -46.0, -46.0,
							-46.0, -46.0, -46.0, -46.0, -46.0, -46.0, -46.0, -46.0, -46.0, -46.0, -46.0, -46.0,
							-46.0, -46.0, -46.0, -46.0, -46.0, -46.0, -46.0, -46.0, -46.0, -46.0,

							//INNER RING
																								  -26.0, -24.0,
							-22.0, -20.0, -18.0, -16.0, -14.0, -12.0, -10.0,  -8.0,  -6.0,  -4.0,  -2.0,   0.0,
							  2.0,   4.0,   6.0,   8.0,  10.0,  12.0,  14.0,  16.0,  18.0,  20.0,  22.0,  24.0,
							 26.0,

							 26.0,  26.0,  26.0,  26.0,  26.0,  26.0,  26.0,  26.0,  26.0,  26.0,  26.0,  26.0,
							 26.0,  26.0,

							 24.0,  22.0,  20.0,  18.0,  16.0,  14.0,  12.0,  10.0,   8.0,   6.0,   4.0,   2.0,
							  0.0,  -2.0,  -4.0,  -6.0,  -8.0, -10.0, -12.0, -14.0, -16.0, -18.0, -20.0, -22.0,
							-24.0, -26.0,

							-26.0, -26.0, -26.0, -26.0, -26.0, -26.0, -26.0, -26.0, -26.0, -26.0, -26.0, -26.0,
							-26.0, -26.0, -26.0, -26.0, -26.0, -26.0, -26.0, -26.0, -26.0, -26.0, -26.0, -26.0,
							-26.0 };

	float cheerios_y[] = {  //OUTER RING
							-46.0, -46.0, -46.0, -46.0, -46.0, -46.0, -46.0, -46.0, -46.0, -46.0, -46.0, -46.0,
							-44.0, -42.0, -40.0, -38.0, -36.0, -34.0, -32.0, -30.0, -28.0, -26.0, -24.0, -22.0,
							-22.0, -22.0, -22.0, -22.0, -22.0, -22.0, -22.0, -22.0,	-22.0, -22.0, -22.0, -22.0,
							-22.0, -22.0, -22.0, -22.0, -22.0, -22.0, -22.0, -22.0,	-22.0, -22.0, -22.0, -22.0,

							-20.0, -18.0, -16.0, -14.0, -12.0, -10.0,  -8.0,  -6.0,  -4.0,  -2.0,   0.0,   2.0,
							  4.0,   6.0,   8.0,  10.0,  12.0,  14.0,  16.0,  18.0,  20.0,  22.0,  24.0,  26.0,
							 28.0,  30.0,  32.0,  34.0,  36.0,  38.0,  40.0,  42.0,  44.0,

							 46.0,  46.0,  46.0,  46.0,  46.0,  46.0,  46.0,  46.0,  46.0,  46.0,  46.0,  46.0,
							 46.0,  46.0,  46.0,  46.0,  46.0,  46.0,  46.0,  46.0,  46.0,  46.0,  46.0,  46.0,
							 46.0,  46.0,  46.0,  46.0,  46.0,  46.0,  46.0,  46.0,  46.0,  46.0,  46.0,  46.0,
							 46.0,  46.0,  46.0,  46.0,  46.0,  46.0,  46.0,  46.0,  46.0,  46.0,  46.0,

							 46.0,  44.0,  42.0,  40.0,  38.0,  36.0,  34.0,  32.0,  30.0,  28.0,  26.0,  24.0,
							 22.0,  20.0,  18.0,  16.0,  14.0,  12.0,  10.0,   8.0,   6.0,   4.0,   2.0,   0.0,
							 -2.0,  -4.0,  -6.0,  -8.0, -10.0, -12.0, -14.0, -16.0, -18.0, -20.0, -22.0, -24.0,
							-26.0, -28.0, -30.0, -32.0, -34.0, -36.0, -38.0, -40.0, -42.0, -44.0,

							//INNER RING
																								  -26.0, -26.0,
							-24.0, -22.0, -20.0, -18.0, -16.0, -14.0, -12.0, -10.0,  -8.0,  -6.0,  -4.0,  -2.0,
							 -2.0,  -2.0,  -2.0,  -2.0,  -2.0,  -2.0,  -2.0,  -2.0,  -2.0,  -2.0,  -2.0,  -2.0,
							 -2.0,

							  0.0,   2.0,   4.0,   6.0,   8.0,  10.0,  12.0,  14.0,  16.0,  18.0,  20.0,  22.0,
							 24.0,  26.0,

							 26.0,  26.0,  26.0,  26.0,  26.0,  26.0,  26.0,  26.0,  26.0,  26.0,  26.0,  26.0,
							 26.0,  26.0,  26.0,  26.0,  26.0,  26.0,  26.0,  26.0,  26.0,  26.0,  26.0,  26.0,
							 26.0,  26.0,

							 24.0,  22.0,  20.0,  18.0,  16.0,  14.0,  12.0,  10.0,   8.0,   6.0,   4.0,   2.0,
							  0.0,  -2.0,  -4.0,  -6.0,  -8.0, -10.0, -12.0, -14.0, -16.0, -18.0, -20.0, -22.0,
							-24.0 };
	for (int i = 0; i < 266; i++) {
		_cheerios.push_back(new Cheerio(cheerios_x[i], cheerios_y[i], 0.75, "Cheerio"));
	}

	float amb[] = { 0.2f, 0.15f, 0.05f, 1.0f };
	float diff[] = { 204.0f / 255.0f , 197.0f / 255.0f, 16.0f / 255.0f, 1.0f };
	float spec[] = { 0.05f, 0.05f, 0.05f, 1.0f };
	float emissive[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininess = 10.0f;
	int texcount = 0;
	objId = 3;
	meshSet(objId, amb, diff, spec, emissive, shininess, texcount);
	createTorus(0.3f, 0.7f, 50, 50);
}

void createButter() {
	float butter_x[] = { -30.0f, 20.0f, 30.0f, -15.0f };
	float butter_y[] = { 20.0f, -20.0f, 30.0f, -25.0f };

	for (int i = 0; i < 4; i++) {
		_butters.push_back(new Butter(butter_x[i], butter_y[i], 0.75f, "Butter"));
	}


	float amb[] = { 0.2f, 0.15f, 0.1f, 1.0f };
	float diff[] = { 222.0 / 255.0f, 237.0 / 255.0f, 61.0 / 255.0f, 0.7f };
	float spec[] = { 0.15f, 0.15f, 0.15f, 1.0f };
	float emissive[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininess = 65.0f;
	int texcount = 0;
	objId = 5;
	meshSet(objId, amb, diff, spec, emissive, shininess, texcount);
	createCube();
}

void createOrange() {

	float orange_x[] = { 10.0f, 12.0f, 21.0f, -15.0f, -22.0f };
	float orange_y[] = { 18.0f, -14.0f, 12.0f, -6.0f, -2.0f };
	for (int i = 0; i < 5; i++) {
		_oranges.push_back(new Orange(orange_x[i], orange_y[i], 2.5, "Orange"));
		_oranges.at(i)->randomizeSpeedX();
		_oranges.at(i)->randomizeSpeedY();
	}

	float amb[] = { 0.2f, 0.15f, 0.1f, 1.0f };
	float diff[] = { 234.0f / 255.0f, 167.0f / 255.0f, 11.0f / 255.0f, 1.0f };
	float spec[] = { 0.02f, 0.02f, 0.02f, 1.0f };
	float emissive[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininess = 10.0f;
	int texcount = 0;
	objId = 6;
	meshSet(objId, amb, diff, spec, emissive, shininess, texcount);
	createSphere(2, 30);
}

void createStencil() {
	float amb[] = { 0.2f, 0.15f, 0.1f, 1.0f };
	float diff[] = { 234.0f / 255.0f, 167.0f / 255.0f, 11.0f / 255.0f, 1.0f };
	float spec[] = { 0.02f, 0.02f, 0.02f, 1.0f };
	float emissive[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininess = 10.0f;
	int texcount = 0;
	objId = 7;
	meshSet(objId, amb, diff, spec, emissive, shininess, texcount);
	createQuad(1.2,1.2);
}
void createQuadBillboard() {
	float amb[] = { 0.2f, 0.15f, 0.1f, 1.0f };
	float diff[] = { 0.8f, 0.6f, 0.4f, 1.0f };
	float spec[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	float emissive[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininess = 100.0f;
	int texcount = 0;
	objId = 8;
	meshSet(objId, amb, diff, spec, emissive, shininess, texcount);
	createQuad(6, 6);
}

void createParticles() {
	int texcount = 0;
	objId = 9;
	mesh[objId].mat.texCount = texcount;
	createQuad(0.5, 0.5);
}

//FIREWORKS
void setParticles(void) {
	GLfloat v, theta, phi;
	int i;

	for (i = 0; i < MAX_PARTICULAS; i++)
	{
		v = 0.8*frand() + 0.2;
		phi = frand()*M_PI;
		theta = 2.0*frand()*M_PI;

		particles[i].x = 14.0f;
		particles[i].y = -12.0f;
		particles[i].z = 5.0f;
		particles[i].vx = v * cos(theta) * sin(phi);
		particles[i].vz = v * cos(phi);
		particles[i].vy = v * sin(theta) * sin(phi);
		particles[i].ax = 0.05f; /* simular um pouco de vento */
		particles[i].az = -0.10f; /* simular a aceleração da gravidade */
		particles[i].ay = 0.0f;

		/* tom amarelado que vai ser multiplicado pela textura que varia entre branco e preto */
		particles[i].r = 0.882f;
		particles[i].g = 0.552f;
		particles[i].b = 0.211f;

		particles[i].life = 1.0f;		/* vida inicial */
		particles[i].fade = 0.005f;	    /* step de decréscimo da vida para cada iteração */
	}
}

//FIREWORKS
void iterateParticles(int value)
{
	int i;
	float h;
	printf("ITERATING ");
	/* Método de Euler de integração de eq. diferenciais ordinárias
	h representa o step de tempo; dv/dt = a; dx/dt = v; e conhecem-se os valores iniciais de x e v */

	h = 0.125f;
	//h = 0.033;
	if (fireworks) {

		for (i = 0; i < MAX_PARTICULAS; i++)
		{
			particles[i].x += (h*particles[i].vx);
			particles[i].y += (h*particles[i].vy);
			particles[i].z += (h*particles[i].vz);
			particles[i].vx += (h*particles[i].ax);
			particles[i].vy += (h*particles[i].ay);
			particles[i].vz += (h*particles[i].az);
			particles[i].life -= particles[i].fade;
		}
		glutPostRedisplay();
		glutTimerFunc(33, iterateParticles, 0);
	}
}

void createLights() {
	float lights_x[] = { 45.0f, 20.0f, 20.0f, -20.0f, -20.0f, -45.0f };
	float lights_y[] = { 45.0f, 20.0f, -20.0f, -20.0f, 20.0f, -45.0f };
	float amb[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float diff[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float spec[] = { 0.5f, 0.5f, 0.5f, 0.5f };
	int texcount = 0;
	objId = 4;

	for (int i = 0; i < POINTLIGHTS_NR; i++) {
		PointLight pl;
		pl.pos[0] = lights_x[i];
		pl.pos[1] = lights_y[i];
		pl.pos[2] = 5.0f;
		pl.pos[3] = 1.0f; // point light; has to be 1
		pl.ambient[0] = amb[0];
		pl.ambient[1] = amb[1];
		pl.ambient[2] = amb[2];
		pl.ambient[3] = amb[3];
		pl.diffuse[0] = diff[0];
		pl.diffuse[1] = diff[1];
		pl.diffuse[2] = diff[2];
		pl.diffuse[3] = diff[3];
		pl.specular[0] = spec[0];
		pl.specular[1] = spec[1];
		pl.specular[2] = spec[2];
		pl.specular[3] = spec[3];
		pl.isEnabled = true;
		pl.isLocal = true;
		pl.isSpot = false;

		pointLights.push_back(pl);
	}
	PointLight pl_right;
	pl_right.pos[0] = _car->getCarX() + 1.5f;
	pl_right.pos[1] = _car->getCarY() - 0.5f;
	pl_right.pos[2] = _car->getCarZ();
	pl_right.pos[3] = 1.0f;
	pl_right.ambient[0] = amb[0];
	pl_right.ambient[1] = amb[1];
	pl_right.ambient[2] = amb[2];
	pl_right.ambient[3] = amb[3];
	pl_right.diffuse[0] = diff[0];
	pl_right.diffuse[1] = diff[1];
	pl_right.diffuse[2] = diff[2];
	pl_right.diffuse[3] = diff[3];
	pl_right.specular[0] = spec[0];
	pl_right.specular[1] = spec[1];
	pl_right.specular[2] = spec[2];
	pl_right.specular[3] = spec[3];
	pl_right.isEnabled = false;
	pl_right.isLocal = true;
	pl_right.isSpot = true;
	//pl_right.cutOff = 45.f;
	pl_right.exponent = 4.0f;
	pl_right.cone = 30.0f;

	pointLights.push_back(pl_right);

	PointLight pl_left;
	pl_left.pos[0] = _car->getCarX() + 1.5f;
	pl_left.pos[1] = _car->getCarY() + 0.5f;
	pl_left.pos[2] = _car->getCarZ();
	pl_left.pos[3] = 1.0f;
	pl_left.ambient[0] = amb[0];
	pl_left.ambient[1] = amb[1];
	pl_left.ambient[2] = amb[2];
	pl_left.ambient[3] = amb[3];
	pl_left.diffuse[0] = diff[0];
	pl_left.diffuse[1] = diff[1];
	pl_left.diffuse[2] = diff[2];
	pl_left.diffuse[3] = diff[3];
	pl_left.specular[0] = spec[0];
	pl_left.specular[1] = spec[1];
	pl_left.specular[2] = spec[2];
	pl_left.specular[3] = spec[3];
	pl_left.isEnabled = false;
	pl_left.isLocal = true;
	pl_left.isSpot = true;
	pl_left.cutOff = 45.0f;
	pl_left.exponent = 4.0f;
	pl_left.cone = 30.0f;

	pointLights.push_back(pl_left);


	for (int i = 0; i < pointLights.size(); i++) {
		printf("Position of index %d is %f %f %f %f\n", i, pointLights[i].pos[0], pointLights[i].pos[1], pointLights[i].pos[2], pointLights[i].pos[3]);
	}
}

void restartGame() {
	// clean objects
	_oranges.clear();
	_cheerios.clear();
	_butters.clear();
	// game is not paused anymore
	pause = false;
	// creates objects in their first locations again
	createCar();
	createOrange();
	createButter();
	createCheerio();

}

// ------------------------------------------------------------
//
// Model loading and OpenGL setup
//

void init()
{
	// set the camera position based on its spherical coordinates
	camX = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camZ = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camY = r * sin(beta * 3.14f / 180.0f);

	glGenTextures(5, TextureArray);
	TGA_Texture(TextureArray, (char*)"textures/stone.tga", 0);
	TGA_Texture(TextureArray, (char*)"textures/checker.tga", 1);
	TGA_Texture(TextureArray, (char*)"textures/lightwood.tga", 2);
	TGA_Texture(TextureArray, (char*)"textures/tree.tga", 3);
	TGA_Texture(TextureArray, (char*)"textures/lensflare.tga", 4);

	createTable();
	createCar();
	createWheel();
	createCheerio();
	createLights();
	createButter();
	createOrange();
	createStencil();
	createQuadBillboard();
	createParticles();

	// some GL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glClearStencil(0x0);
	
	

}


// ------------------------------------------------------------
//
// Main function
//


int main(int argc, char **argv) {

	//  GLUT initialization
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE | GLUT_STENCIL);

	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);

	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WinX, WinY);
	WindowHandle = glutCreateWindow(CAPTION);


	//  Callback Registration
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);

	//	Mouse and Keyboard Callbacks
	glutKeyboardFunc(processKeys);
	glutKeyboardUpFunc(keyUp);
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);
	glutMouseWheelFunc(mouseWheel);

	glutTimerFunc(0, timer, 0);
	//glutIdleFunc(renderScene);  // Use it for maximum performance
	glutTimerFunc(0, refresh, 0);    //use it to to get 60 FPS whatever

	//	return from main loop
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	//	Init GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));
	printf("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	if (!setupShaders())
		return(1);

	init();

	//  GLUT main loop
	glutMainLoop();

	return(0);

}


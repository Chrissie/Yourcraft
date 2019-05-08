#include <Windows.h>
#include <GL/freeglut.h>
#include <cstdio>
#define _USE_MATH_DEFINES
#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "ObjModel.h"
#include <cmath>
#include <list>
#include <vector>

#define CUBE_GRASS_TOP 1
#define CUBE_DIRT 2
#define CUBE_AIR -1

float lastFrameTime = 0;

int width, height;

int picWidth, picHeight, bpp;
unsigned char* imgData;
int xOff = 0, yOff = 0;
int sub_width = 16, sub_height = 16;

int world[64][32][64];

std::vector<std::pair<int, ObjModel*> > models;
int currentModel = 0;

const char* terrainFile = "terrain.png";
const char* steveFile = "models/steve/steve.png";
GLuint *textures = new GLuint[2];

struct Camera
{
	float posX = -64;
	float posY = -10;
	float posZ = -64;
	float rotX = 0;
	float rotY = 0;
	float rotZ = 0;
} camera;

bool keys[255];

void initGraphics(void)
{
	glEnable(GL_DEPTH_TEST);

	//models.push_back(std::pair<int, ObjModel*>(75, new ObjModel("models/car/honda_jazz.obj")));
	////models.push_back(std::pair<int, ObjModel*>(5, new ObjModel("models/auto/auto.obj")));
	//models.push_back(std::pair<int, ObjModel*>(1, new ObjModel("models/bloemetje/PrimRoseP.obj")));
	//models.push_back(std::pair<int, ObjModel*>(1, new ObjModel("models/cube/cube-textures.obj")));
	//models.push_back(std::pair<int, ObjModel*>(35, new ObjModel("models/ship/shipA_OBJ.obj")));
	//models.push_back(std::pair<int, ObjModel*>(3, new ObjModel("models/steve/steve.obj")));
}

void drawCube(int cubeType)
{
	if (cubeType == CUBE_AIR) return;
	
	float xStart = 0, yStart = 0, xEnd = 0, yEnd = 0;
	if (cubeType == CUBE_GRASS_TOP)
	{
		//TOP SIDE
		xEnd = yEnd = sub_width / (picWidth*1.00f);
		glBegin(GL_QUADS);
		glTexCoord2f(xStart, yStart);	glVertex3f(-1, 1, -1);
		glTexCoord2f(xStart, yEnd);	glVertex3f(1, 1, -1);
		glTexCoord2f(xEnd, yEnd);	glVertex3f(1, 1, 1);
		glTexCoord2f(xEnd, yStart);	glVertex3f(-1, 1, 1);
		glEnd();

		//BOTTOM SIDE
		xStart = 2 * xEnd;
		xEnd = 3 * yEnd;
		glBegin(GL_QUADS);
		glTexCoord2f(xStart, yStart);	glVertex3f(-1, -1, -1);
		glTexCoord2f(xStart, yEnd);	glVertex3f(1, -1, -1);
		glTexCoord2f(xEnd, yEnd);	glVertex3f(1, -1, 1);
		glTexCoord2f(xEnd, yStart);	glVertex3f(-1, -1, 1);
		glEnd();

		//BACK SIDE
		xStart += yEnd;
		xEnd += yEnd;
		glBegin(GL_QUADS);
		glTexCoord2f(xStart, yStart);	glVertex3f(-1, 1, -1);
		glTexCoord2f(xStart, yEnd);	glVertex3f(-1, -1, -1);
		glTexCoord2f(xEnd, yEnd);	glVertex3f(1, -1, -1);
		glTexCoord2f(xEnd, yStart);	glVertex3f(1, 1, -1);

		//FRONT SIDE
		glTexCoord2f(xStart, yStart);	glVertex3f(-1, 1, 1);
		glTexCoord2f(xStart, yEnd);		glVertex3f(-1, -1, 1);
		glTexCoord2f(xEnd, yEnd);		glVertex3f(1, -1, 1);
		glTexCoord2f(xEnd, yStart);		glVertex3f(1, 1, 1);

		//LEFT SIDE
		glTexCoord2f(xStart, yStart);	glVertex3f(-1, 1, 1);
		glTexCoord2f(xStart, yEnd);	glVertex3f(-1, -1, 1);
		glTexCoord2f(xEnd, yEnd);	glVertex3f(-1, -1, -1);
		glTexCoord2f(xEnd, yStart);	glVertex3f(-1, 1, -1);

		//RIGHT SIDE
		glTexCoord2f(xStart, yStart);	glVertex3f(1, 1, 1);
		glTexCoord2f(xStart, yEnd);	glVertex3f(1, -1, 1);
		glTexCoord2f(xEnd, yEnd);	glVertex3f(1, -1, -1);
		glTexCoord2f(xEnd, yStart);	glVertex3f(1, 1, -1);

		glEnd();
		xOff = 0;
	}
	else if (cubeType == CUBE_DIRT)
	{
		//BOTTOM SIDE
		yEnd = sub_width / (picWidth*1.00f);
		xStart = yEnd * 2;
		xEnd = yEnd * 3;
		glBegin(GL_QUADS);
		glTexCoord2f(xStart, yStart);	glVertex3f(-1, -1, -1);
		glTexCoord2f(xStart, yEnd);	glVertex3f(1, -1, -1);
		glTexCoord2f(xEnd, yEnd);	glVertex3f(1, -1, 1);
		glTexCoord2f(xEnd, yStart);	glVertex3f(-1, -1, 1);

		//TOP SIDE
		glTexCoord2f(xStart, yStart);	glVertex3f(-1, 1, -1);
		glTexCoord2f(xStart, yEnd);	glVertex3f(1, 1, -1);
		glTexCoord2f(xEnd, yEnd);	glVertex3f(1, 1, 1);
		glTexCoord2f(xEnd, yStart);	glVertex3f(-1, 1, 1);

		//BACK SIDE
		glTexCoord2f(xStart, yStart);	glVertex3f(-1, 1, -1);
		glTexCoord2f(xStart, yEnd);	glVertex3f(-1, -1, -1);
		glTexCoord2f(xEnd, yEnd);	glVertex3f(1, -1, -1);
		glTexCoord2f(xEnd, yStart);	glVertex3f(1, 1, -1);

		//FRONT SIDE
		glTexCoord2f(xStart, yStart);	glVertex3f(-1, 1, 1);
		glTexCoord2f(xStart, yEnd);	glVertex3f(-1, -1, 1);
		glTexCoord2f(xEnd, yEnd);	glVertex3f(1, -1, 1);
		glTexCoord2f(xEnd, yStart);	glVertex3f(1, 1, 1);

		//LEFT SIDE
		glTexCoord2f(xStart, yStart);	glVertex3f(-1, 1, 1);
		glTexCoord2f(xStart, yEnd);	glVertex3f(-1, -1, 1);
		glTexCoord2f(xEnd, yEnd);	glVertex3f(-1, -1, -1);
		glTexCoord2f(xEnd, yStart);	glVertex3f(-1, 1, -1);

		//RIGHT SIDE
		glTexCoord2f(xStart, yStart);	glVertex3f(1, 1, 1);
		glTexCoord2f(xStart, yEnd);	glVertex3f(1, -1, 1);
		glTexCoord2f(xEnd, yEnd);	glVertex3f(1, -1, -1);
		glTexCoord2f(xEnd, yStart);	glVertex3f(1, 1, -1);

		glEnd();
		xOff = 0;
	}
}

void display()
{
	glClearColor(0.6f, 0.6f, 1, 1);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, (float)width/height, 0.1, 1000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(camera.rotX, 1, 0, 0);
	glRotatef(camera.rotZ, 0, 1, 0);
	//glRotatef(camera.rotZ, 0, 0, 1);
	glTranslatef(camera.posX, camera.posY, camera.posZ);
	
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindTexture(GL_TEXTURE_2D, textures[0]);
	for (int y = 0; y < 16; y++)
	{ 
		for (int x = 0; x < 64; x++)
		{
			for (int z = 0; z < 64; z++)
			{
				glPushMatrix();
				glTranslatef((float)x * 2, (float)y * 2 , (float)z * 2 );
				drawCube(world[x][y][z]);
				glPopMatrix();
			}
		}
	}
	//glBindTexture(GL_TEXTURE_2D, textures[1]);
	//models[currentModel].second->draw();

	glutSwapBuffers();
}

void move(float angle, float fac)
{
	camera.posX += (float)cos((camera.rotZ + angle) / 180 * M_PI) * fac;
	camera.posZ += (float)sin((camera.rotZ + angle) / 180 * M_PI) * fac;
}

void idle()
{
	float frameTime = glutGet(GLUT_ELAPSED_TIME)/1000.0f;
	float deltaTime = frameTime - lastFrameTime;
	lastFrameTime = frameTime;

	const float speed = 15;
	if (keys['a']) move(0, deltaTime*speed);
	if (keys['d']) move(180, deltaTime*speed);
	if (keys['w']) move(90, deltaTime*speed);
	if (keys['s']) move(270, deltaTime*speed);
	if (keys['q']) camera.posY += 0.5f;
	if (keys['z']) camera.posY -= 0.5f;

	glutPostRedisplay();
}

bool justMovedMouse = false;
void mousePassiveMotion(int x, int y)
{
	int dx = x - width / 2;
	int dy = y - height / 2;
	if ((dx != 0 || dy != 0) && abs(dx) < 400 && abs(dy) < 400 && !justMovedMouse)
	{
		camera.rotZ += dx / 10.0f;
		camera.rotX += dy / 10.0f;
	}
	if (!justMovedMouse)
	{
		glutWarpPointer(width / 2, height / 2);
		justMovedMouse = true;
	}
	else
		justMovedMouse = false;
}

void keyboard(unsigned char key, int, int)
{
	if (key == 27)
		exit(0);
	keys[key] = true;
}

void keyboardUp(unsigned char key, int,int)
{
	keys[key] = false;
}

int main(int argc, char* argv[])
{
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInit(&argc, argv);
	glutCreateWindow("Plastic (fake) Minecraft");

	initGraphics();

	glGenTextures(1, textures);
	imgData = stbi_load(terrainFile, &picWidth, &picHeight, &bpp, 4);	
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D,	0, GL_RGBA,	picWidth, picHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
	stbi_image_free(imgData);

	/*stbi_set_flip_vertically_on_load(1);
	imgData = stbi_load(steveFile, &picWidth, &picHeight, &bpp, 4);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, picWidth, picHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	stbi_image_free(imgData);*/


	int randHeight = 0;
	for (int i = 0; i < 64; i++)
	{
		for (int j = 0; j < 64; j++)
		{
			randHeight = (int)(stb_perlin_noise3((float)i/64.0f, (float)j/64.0f, 1, 0, 0, 0) * 32) + 16;
			for (int k = 0; k < 16; k++)
			{
				if (k < randHeight)
				{
					world[i][k][j] = CUBE_DIRT;
				}
				else if (k == randHeight)
				{
					world[i][k][j] = CUBE_GRASS_TOP;
				}
				else world[i][k][j] = CUBE_AIR;
			}
		}
		
	}

	memset(keys, 0, sizeof(keys));
	glEnable(GL_DEPTH_TEST);

	glutIdleFunc(idle);
	glutDisplayFunc(display);
	glutReshapeFunc([](int w, int h) { width = w; height = h; glViewport(0, 0, w, h); });
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardUp);
	glutPassiveMotionFunc(mousePassiveMotion);


	glutWarpPointer((width / 2),( height / 2));
	glutMainLoop();


	return 0;
}
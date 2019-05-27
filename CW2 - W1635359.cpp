//3D Graphics Programming CW2 - Joseph Calabro - W1635359

#include <fstream>
#include <string> 
#include <cstdlib>
#include <vector>
#include <math.h>		
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <iostream>	//Needed for console output (debugging)
#include <gl/freeglut.h>
#include <iostream>

#ifdef WIN32
#include "gltools.h"
#include <windows.h>		// Must have for Windows platform builds
#include <gl\gl.h>			// Microsoft OpenGL headers (version 1.1 by themselves)
#include <gl\glu.h>			// OpenGL Utilities
#include "glm.h"
#endif

#define IMAGE1      0
#define IMAGE2      1
#define IMAGE3      2
#define IMAGE4      3
#define IMAGE5      4
#define IMAGE6      5
#define IMAGE7      6
#define IMAGE8      7
#define IMAGE9      8
#define IMAGE10     9
#define IMAGE11     10
#define IMAGE12     11
#define IMAGE13     12
#define IMAGE14     13
#define TEXTURE_COUNT 14
GLuint  textures[TEXTURE_COUNT];
GLfloat shine = 500;

//below is simply a character array to hold the file names
//note that you may need to replace the below with the full directory root depending on where you put your image files
//if you put them where the exe is then you just need the name as below - THESE IMAGES  ARE IN THE DEBUG FOLDER, YOU CAN ADD ANY NEW ONES THERE 
const char *textureFiles[TEXTURE_COUNT] = { "brick_texture_lo_res.tga", "FootballCompleteMap.tga", "targetGreen.tga", "targetRed.tga", "targetBlue.tga", "targetDull.tga", "stormydays_large.tga", "fillBarHorizontal.tga", "fillBarVerticalR.tga","FootballCompleteMap.tga", "orangeFlowerFinal5.tga", "yellowFlowerFinal.tga", "grass_diff(1).tga", "palmBranchA.tga"};


//for lighting if you want to experiment with these
GLfloat mKa[4] = { 0.11f,0.06f,0.11f,1.0f }; //ambient
GLfloat mKd[4] = { 0.43f,0.47f,0.54f,1.0f }; //diffuse
GLfloat mKs[4] = { 1.0f,1.0f,1.0f,1.0f }; //specular
GLfloat  specref[] = { 800.0f, 800.0f, 800.0f, 800.0f };
GLfloat mKe[4] = { 0.5f,0.5f,0.0f,1.0f }; //emission

//// Useful lighting colour value
GLfloat  whiteLightBright[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat  redLight[] = { 1.0f, 0.0f, 0.0f, 1.0f };
GLfloat  whiteLightLessBright[] = { 0.8f, 0.8f, 0.8f, 1.0f };
//GLfloat	 lightPos[] = { 100.0f, 100.0f, 50.0f, 1.0f };


//we need these for the texture loader
//they are to do with the image format and size
GLint iWidth, iHeight, iComponents;
GLenum eFormat;
// this is a pointer to memory where the image bytes will be held 
GLbyte *pBytes0;

///////////////////////////////////////////////////////////////////////////////////////////////////////

int window;

// Lighting values
GLfloat  whiteLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat  sourceLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat	 lightPos[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat	 lightDir[] = { 0.0f, 0.0f, -1.0f, 0.0f };

// Earth and Moon angle of revolution
GLfloat fMoonRot = 0.0f;
GLfloat fMarsRot = 0.0f;
GLfloat fEarthRot = 0.0f;

GLfloat sunZ = -150.0f;
GLfloat sunX = 0.0f;
GLfloat sunY = -20.0f;

GLfloat sunMov_X = 0.0f; // 1 - 9
GLfloat sunMov_Y = 1.0f; // 3 - 9
GLfloat sunMov_Yy = 0.0f;
GLfloat gav = 9.81f;
GLfloat spd = 10.0f;

GLint score = 0, counter = 0, hitCount = 0;
bool move = false, follow = false, gameOver = false;


//------------------------------------//

void CamFollow()
{
	if (follow)
	{
		gluLookAt(sunX, sunY + 20, sunZ + 150,
			sunX, sunY + 20, sunZ,
			0.0f, 1.0f, 0.0f);
	}
}

class Target
{
private:
	GLint points;
	GLfloat xPos, yPos, zPos;
	int colour;
	

public:
	bool hit = false;
	Target(GLfloat x, GLfloat y, GLint pnts, int col)
	{
		xPos = x;
		yPos = y;
		zPos = -290.0f;
		colour = col;

		points = pnts;
	}
	Target()
	{
	}

	void collision()
	{
		GLfloat ballX = sunX + 10;
		GLfloat ballY = sunY - 15;
		GLfloat ballZ = sunZ + 150;

		GLfloat xCom = (xPos - ballX)*(xPos - ballX);
		GLfloat yCom = (yPos - ballY)*(yPos - ballY);
		GLfloat zCom = (zPos - ballZ)*(zPos - ballZ);

		zCom += 60;

		GLint allCom = xCom + yCom + zCom ;

		GLfloat distance = sqrt(allCom);

		if (distance <= 35.0f)
		{
			if (!hit)
			{
				score += points;
				hitCount++;
			}

			hit = true;
		}
		

	}

	void draw()
	{
		glLoadIdentity();
		glTranslatef(0.0, 20.0f, -149.0f); //Set Pos

		CamFollow(); //Camra Follow

		//glRotatef(90, 0.0f, 1.0f, 0.0f);
		glTranslatef(xPos, yPos, -290.0f + 50.0f);

		glColor3ub(0, 0, 255);

		GLUquadricObj *quadric2;
		quadric2 = gluNewQuadric();
		gluQuadricDrawStyle(quadric2, GLU_LINE);

		if (colour == 1)
		{
			if (!hit)
				drawTexGreen(IMAGE3);

			if (hit)
				drawTexGreen(IMAGE6);
		}

		if (colour == 2)
		{
			if (!hit)
				drawTexRed(IMAGE4);

			if (hit)
				drawTexRed(IMAGE6);
		}
		
		if (colour == 3)
		{
			if (!hit)
				drawTexBlue(IMAGE5);

			if (hit)
				drawTexBlue(IMAGE6);
		}

		//gluDisk(quadric2, 15.0, 15.0, 20, 20);
	}

	void drawTexGreen(int image)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//add some lighting normals for each vertex
		//draw the texture on the front
		glEnable(GL_TEXTURE_2D);
		// glFrontFace(GL_CW); //use glFrontFace(GL_CW) to texture the other side - not needed here as we set this elsewhere
		glColor3f(0.8, 0.8, 0.8);
		glEnable(GL_TEXTURE_2D);

		//bind the texture 
		glBindTexture(GL_TEXTURE_2D, textures[image]);
		glBegin(GL_QUADS);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-20.0, -20.0, 0.0);//
		glTexCoord3f(1.0, 0.0, 0.0);
		glVertex3f(20.0, -20.0, 0.0);//
		glTexCoord2f(1.0, 1.0);
		glVertex3f(20.0, 20.0, 0.0);//
		glTexCoord2f(0.0, 1.0);
		glVertex3f(-20.0, 20.0, 0.0);//

		glEnd();
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
	}

	void drawTexRed(int image)
	{

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//add some lighting normals for each vertex
		//draw the texture on the front
		glEnable(GL_TEXTURE_2D);
		// glFrontFace(GL_CW); //use glFrontFace(GL_CW) to texture the other side - not needed here as we set this elsewhere
		glColor3f(0.8, 0.8, 0.8);
		glEnable(GL_TEXTURE_2D);

		//bind the texture 
		glBindTexture(GL_TEXTURE_2D, textures[image]);
		glBegin(GL_QUADS);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-15.0, -15.0, 0.0);//
		glTexCoord3f(1.0, 0.0, 0.0);
		glVertex3f(15.0, -15.0, 0.0);//
		glTexCoord2f(1.0, 1.0);
		glVertex3f(15.0, 15.0, 0.0);//
		glTexCoord2f(0.0, 1.0);
		glVertex3f(-15.0, 15.0, 0.0);//

		glEnd();
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
	}

	void drawTexBlue(int image)
	{

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//add some lighting normals for each vertex
		//draw the texture on the front
		glEnable(GL_TEXTURE_2D);
		// glFrontFace(GL_CW); //use glFrontFace(GL_CW) to texture the other side - not needed here as we set this elsewhere
		glColor3f(0.8, 0.8, 0.8);
		glEnable(GL_TEXTURE_2D);

		//bind the texture 
		glBindTexture(GL_TEXTURE_2D, textures[image]);
		glBegin(GL_QUADS);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-15.0, -15.0, 0.0);//
		glTexCoord3f(1.0, 0.0, 0.0);
		glVertex3f(15.0, -15.0, 0.0);//
		glTexCoord2f(1.0, 1.0);
		glVertex3f(15.0, 15.0, 0.0);//
		glTexCoord2f(0.0, 1.0);
		glVertex3f(-15.0, 15.0, 0.0);//

		glEnd();
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
	}

};
std::vector<Target> Vtargets(0);

void createTargets()
{

	Target t = Target(-40.0f, -15.0f, 10, 1);
	Vtargets.push_back(t);

	Target t1 = Target(40.0f, -15.0f, 10, 1);
	Vtargets.push_back(t1);

	Target t2 = Target(0.0f, 30.0f, 30, 2);
	Vtargets.push_back(t2);

	Target t3 = Target(-75.0f, 30.0f, 20, 3);
	Vtargets.push_back(t3);

	Target t4 = Target(75.0f, 30.0f, 20, 3);
	Vtargets.push_back(t4);

	Target t5 = Target(-110.0f, -15.0f, 30, 2);
	Vtargets.push_back(t5);

	Target t6 = Target(110.0f, -15.0f, 30, 2);
	Vtargets.push_back(t6);
}

void drawText(char *string, float x, float y, GLfloat r, GLfloat g, GLfloat b)
{
	glColor3f(r, g, b);

	char *c; 
	glRasterPos2f(x, y);

	for (c = string; *c != '\0'; c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c );
	}
}

void drawUIH(int image)
{
	//add some lighting normals for each vertex
	//draw the texture on the front
	glEnable(GL_TEXTURE_2D);
	//glFrontFace(GL_CW); //use glFrontFace(GL_CW) to texture the other side - not needed here as we set this elsewhere
	glColor3f(0.8, 0.8, 0.8);
	glEnable(GL_TEXTURE_2D);
	//bind the texture 
	glBindTexture(GL_TEXTURE_2D, textures[image]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);

	glTexCoord2f(0.0, 0.0);
	glVertex2f(0.0, 0.0);//

	glTexCoord2f(0.0, 1.0);
	glVertex2f(0.0, 0.1);//

	glTexCoord2f(1.0, 1.0);
	glVertex2f(0.2, 0.1);//

	glTexCoord2f(1.0, 0.0);
	glVertex2f(0.2, 0.0);//

	//glFrontFace(GL_CCW);

	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void drawUIV(int image)
{
	//add some lighting normals for each vertex
	//draw the texture on the front
	glEnable(GL_TEXTURE_2D);
	//glFrontFace(GL_CW); //use glFrontFace(GL_CW) to texture the other side - not needed here as we set this elsewhere
	glColor3f(0.8, 0.8, 0.8);
	glEnable(GL_TEXTURE_2D);
	//bind the texture 
	glBindTexture(GL_TEXTURE_2D, textures[image]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);

	glTexCoord2f(0.0, 0.0);
	glVertex2f(1.0, 1.0);//

	glTexCoord2f(0.0, 1.0);
	glVertex2f(1.0, 0.65);//

	glTexCoord2f(1.0, 1.0);
	glVertex2f(0.93, 0.65);//

	glTexCoord2f(1.0, 0.0);
	glVertex2f(0.93, 1.0);//


	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void drawMarker(int image)
{
	glColor3ub(255, 0, 255);
	//add some lighting normals for each vertex
	//draw the texture on the front
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	//bind the texture 
	glBindTexture(GL_TEXTURE_2D, textures[image]);
	glBegin(GL_QUADS);


	glNormal3f(0.0f, 0.0f, 1.0f);

	glVertex2f(0.0, 0.0);//

	glVertex2f(0.0, 0.1);//

	glVertex2f(0.01, 0.1);//

	glVertex2f(0.01, 0.0);//

	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void drawMarker2(int image)
{
	glColor3ub(255, 0, 255);
	//add some lighting normals for each vertex
	//draw the texture on the front
	glEnable(GL_TEXTURE_2D);
	//glFrontFace(GL_CW); //use glFrontFace(GL_CW) to texture the other side - not needed here as we set this elsewhere
	//glColor3f(0.0, 1.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	//bind the texture 
	glBindTexture(GL_TEXTURE_2D, textures[image]);
	glBegin(GL_QUADS);


	glNormal3f(0.0f, 0.0f, 1.0f);

	glVertex2f(1.0, 1.0);//

	glVertex2f(1.0, 1.0 - 0.02);//

	glVertex2f(0.93, 1.0 - 0.02);//

	glVertex2f(0.93, 1.0);//

	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void drawHUD()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	/////////////////////////////////////////////////////////////////////

	std::string scr = std::to_string(score);
	int n = scr.length();

	char char_array[5];

	for (int i = 0; i < n; i++)
	{
		char_array[i] = scr[i];
	}

	drawText("SCORE: ", 0.4 + 0.05, 0.2, 1.0, 0.0, 0.0);
	drawText(char_array, 0.48 + 0.05, 0.2, 1.0, 0.0, 0.0);

	scr = std::to_string(10 - counter);
	n = scr.length();

	char char_array2[10];

	for (int i = 0; i < n; i++)
	{
		char_array2[i] = scr[i];
	}

	drawText("Moves Left: ", 0.2 + 0.05, 0.2, 0.0, 1.0, 0.0);
	drawText(char_array2, 0.315 + 0.05, 0.2, 0.0, 1.0, 0.0);

	glLoadIdentity();



	if (gameOver)
	{
		drawText("GAME OVER", 0.4 + 0.05, 0.4, 0.0, 1.0, 1.0);
		drawText("PRESS 'R' TO RESTART", 0.34 + 0.05, 0.45, 0.0, 1.0, 1.0);
		drawText("GAME OVER", 0.4 + 0.05, 0.4 + 0.0025, 1.0, 0.0, 0.0);
		drawText("PRESS 'R' TO RESTART", 0.34 + 0.05, 0.45+0.0025, 1.0, 0.0, 0.0);
	}

	/////////////////////////////////////////////////////////////////////

	if (follow)
	{
		drawText("Follow: 1 ", 0.6 + 0.05, 0.2, 0.0, 0.0, 1.0);
	}
	else
	{
		drawText("Follow: 0 ", 0.6 + 0.05, 0.2, 0.0, 0.0, 1.0);
	}

	/////////////////////////////////////////////////////////////////////

	glLoadIdentity();
	glTranslatef(0.12 + (sunMov_X)/100, 0.05f, 0.0f); //----> Horizontal Marker
	drawMarker(IMAGE1);

	glLoadIdentity();
	glTranslatef(-0.04, -0.03f - (sunMov_Y) / 30, 0.0f); //----> Vertical Marker
	drawMarker2(IMAGE1);

	glLoadIdentity();
	glTranslatef(0.025f, 0.025f, 0.0f); //----> Horizontal Bar
	drawUIH(IMAGE8);

	glLoadIdentity();
	glTranslatef(-0.025, -0.025f, 0.0f); //----> Vertical Bar
	drawUIV(IMAGE9);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

}

void drawBackWall(int image)
{
	//add some lighting normals for each vertex
	//draw the texture on the front
	glEnable(GL_TEXTURE_2D);
	// glFrontFace(GL_CW); //use glFrontFace(GL_CW) to texture the other side - not needed here as we set this elsewhere
	glColor3f(0.8, 0.8, 0.8);
	glEnable(GL_TEXTURE_2D);
	//bind the texture 
	glBindTexture(GL_TEXTURE_2D, textures[image]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-140.0, 0.0, -200.0);//
	glTexCoord3f(1.0, 0.0, 0.0);
	glVertex3f(140.0, 0.0, -200.0);//
	glTexCoord2f(1.0, 1.0);
	glVertex3f(140.0, 105.0, -200.0);//
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-140.0, 105.0, -200.0);//

	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void drawLeftWall(int image)
{
	//add some lighting normals for each vertex
	//draw the texture on the front
	glEnable(GL_TEXTURE_2D);
	// glFrontFace(GL_CW); //use glFrontFace(GL_CW) to texture the other side - not needed here as we set this elsewhere
	glColor3f(0.8, 0.8, 0.8);
	glEnable(GL_TEXTURE_2D);
	//bind the texture 
	glBindTexture(GL_TEXTURE_2D, textures[image]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-139.5, 0.0, 220.0);//
	glTexCoord3f(1.0, 0.0, 0.0);
	glVertex3f(-139.5, 0.0, -220.0);//
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-139.5, 105.0, -220.0);//
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-139.5, 105.0, 220.0);//

	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void drawRightWall(int image)
{
	//add some lighting normals for each vertex
	//draw the texture on the front
	glEnable(GL_TEXTURE_2D);
	// glFrontFace(GL_CW); //use glFrontFace(GL_CW) to texture the other side - not needed here as we set this elsewhere
	glColor3f(0.8, 0.8, 0.8);
	glEnable(GL_TEXTURE_2D);
	//bind the texture 
	glBindTexture(GL_TEXTURE_2D, textures[image]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(139.5, 0.0, -220.0);//
	glTexCoord3f(1.0, 0.0, 0.0);
	glVertex3f(139.5, 0.0, 220.0);//
	glTexCoord2f(1.0, 1.0);
	glVertex3f(139.5, 105.0, 220.0);//
	glTexCoord2f(0.0, 1.0);
	glVertex3f(139.5, 105.0, -220.0);//

	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void drawFloor(int image)
{
	//add some lighting normals for each vertex
	//draw the texture on the front
	glEnable(GL_TEXTURE_2D);
	// glFrontFace(GL_CW); //use glFrontFace(GL_CW) to texture the other side - not needed here as we set this elsewhere
	glColor3f(0.8, 0.8, 0.8);
	glEnable(GL_TEXTURE_2D);
	//bind the texture 
	glBindTexture(GL_TEXTURE_2D, textures[image]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(200.0, 0.0, -200.0);//
	glTexCoord3f(1.0, 0.0, 0.0);
	glVertex3f(-200.0, 0.0, -200.0);//
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-200.0, 0.0, 250.0);//
	glTexCoord2f(0.0, 1.0);
	glVertex3f(200.0, 0.0, 250.0);//

	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void drawSkyBox(int image)
{
	//add some lighting normals for each vertex
//draw the texture on the front
	glEnable(GL_TEXTURE_2D);
	// glFrontFace(GL_CW); //use glFrontFace(GL_CW) to texture the other side - not needed here as we set this elsewhere
	glColor3f(0.8, 0.8, 0.8);
	glEnable(GL_TEXTURE_2D);
	//bind the texture 
	glBindTexture(GL_TEXTURE_2D, textures[image]);
	glBegin(GL_QUADS);

	glFrontFace(GL_CW);

	glNormal3f(0.0f, 0.0f, 1.0f);

	//--//--//--//--//--//--//--//

	//glColor3f(0.0f, 1.0f, 0.5f);    // Color Blue

	glTexCoord2f(0.0, 0.3);
	glVertex3f(-1.0f, -1.0f, 1.0f);    // Top Right Of The Quad (Top)

	glTexCoord3f(0.6, 0.3, 0.0);
	glVertex3f(1.0f, -1.0f, 1.0f);    // Top Left Of The Quad (Top)

	glTexCoord2f(0.6, 0.6);
	glVertex3f(1.0f, -1.0f, -1.0f);    // Bottom Left Of The Quad (Top)

	glTexCoord2f(0.3, 0.6);
	glVertex3f(-1.0f, -1.0f, -1.0f);    // Bottom Right Of The Quad (Top)

	//--//--//--//--//--//--//--//

	//glColor3f(1.0f, 0.5f, 0.5f);    // Color Orange

	glTexCoord2f(0.0, 0.3);
	glVertex3f(-1.0f, 1.0f, -1.0f);    // Top Right Of The Quad (Bottom)

	glTexCoord3f(0.6, 0.3, 0.0);
	glVertex3f(1.0f, 1.0f, -1.0f);    // Top Left Of The Quad (Bottom)

	glTexCoord2f(0.6, 0.6);
	glVertex3f(1.0f, 1.0f, 1.0f);    // Bottom Left Of The Quad (Bottom)

	glTexCoord2f(0.3, 0.6);
	glVertex3f(-1.0f, 1.0f, 1.0f);    // Bottom Right Of The Quad (Bottom)

	//--//--//--//--//--//--//--//

	//glColor3f(1.0f, 0.0f, 0.5f);    // Color Red   

	glTexCoord2f(0.0, 0.3);
	glVertex3f(-1.0f, -1.0f, -1.0f);    // Top Right Of The Quad (Front)

	glTexCoord3f(0.25, 0.3, 0.0);
	glVertex3f(1.0f, -1.0f, -1.0f);    // Top Left Of The Quad (Front)

	glTexCoord2f(0.25, 0.6);
	glVertex3f(1.0f, 1.0f, -1.0f);    // Bottom Left Of The Quad (Front)

	glTexCoord2f(0.0, 0.6);
	glVertex3f(-1.0f, 1.0f, -1.0f);    // Bottom Right Of The Quad (Front)

	//--//--//--//--//--//--//--//

	//glColor3f(1.0f, 1.0f, 0.5f);    // Color Yellow

	glTexCoord2f(0.75, 0.6);
	glVertex3f(-1.0f, 1.0f, 1.0f);    // Top Right Of The Quad (Back)

	glTexCoord3f(0.5, 0.6, 0.0);
	glVertex3f(1.0f, 1.0f, 1.0f);    // Top Left Of The Quad (Back)

	glTexCoord2f(0.5, 0.3);
	glVertex3f(1.0f, -1.0f, 1.0f);    // Bottom Left Of The Quad (Back)

	glTexCoord2f(0.75, 0.3);
	glVertex3f(-1.0f, -1.0f, 1.0f);    // Bottom Right Of The Quad (Back)

	//--//--//--//--//--//--//--//

	//glColor3f(0.0f, 0.0f, 0.5f);    // Color Blue

	glTexCoord2f(0.25, 0.3);
	glVertex3f(1.0f, -1.0f, -1.0f);    // Top Right Of The Quad (Left)

	glTexCoord3f(0.5, 0.3, 0.0);
	glVertex3f(1.0f, -1.0f, 1.0f);    // Top Left Of The Quad (Left)

	glTexCoord2f(0.5, 0.6);
	glVertex3f(1.0f, 1.0f, 1.0f);    // Bottom Left Of The Quad (Left)

	glTexCoord2f(0.25, 0.6);
	glVertex3f(1.0f, 1.0f, -1.0f);    // Bottom Right Of The Quad (Left)

	//--//--//--//--//--//--//--//

	//glColor3f(1.0f, 0.0f, 0.5f);    // Color Violet

	glTexCoord2f(0.75, 0.3);
	glVertex3f(-1.0f, -1.0f, 1.0f);    // Top Right Of The Quad (Right)

	glTexCoord3f(1.0, 0.3, 0.0);
	glVertex3f(-1.0f, -1.0f, -1.0f);    // Top Left Of The Quad (Right)

	glTexCoord2f(1.0, 0.6);
	glVertex3f(-1.0f, 1.0f, -1.0f);    // Bottom Left Of The Quad (Right)

	glTexCoord2f(0.75, 0.6);
	glVertex3f(-1.0f, 1.0f, 1.0f);

	//--//--//--//--//--//--//--//

	glFrontFace(GL_CCW);

	glEnd();
	glDisable(GL_TEXTURE_2D);

}

void drawFlower(int image)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//add some lighting normals for each vertex
	//draw the texture on the front
	glEnable(GL_TEXTURE_2D);
	// glFrontFace(GL_CW); //use glFrontFace(GL_CW) to texture the other side - not needed here as we set this elsewhere
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);

	//bind the texture 
	glBindTexture(GL_TEXTURE_2D, textures[image]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-10.0, -10.0, 0.0);//
	glTexCoord3f(1.0, 0.0, 0.0);
	glVertex3f(10.0, -10.0, 0.0);//
	glTexCoord2f(1.0, 1.0);
	glVertex3f(10.0, 10.0, 0.0);//
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-10.0, 10.0, 0.0);//

	glEnd();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}

void drawPalm(int image)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//add some lighting normals for each vertex
	//draw the texture on the front
	glEnable(GL_TEXTURE_2D);
	// glFrontFace(GL_CW); //use glFrontFace(GL_CW) to texture the other side - not needed here as we set this elsewhere
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);

	//bind the texture 
	glBindTexture(GL_TEXTURE_2D, textures[image]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-10.0, -10.0, 0.0);//
	glTexCoord3f(1.0, 0.0, 0.0);
	glVertex3f(10.0, -10.0, 0.0);//
	glTexCoord2f(1.0, 1.0);
	glVertex3f(10.0, 80.0, 0.0);//
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-10.0, 80.0, 0.0);//

	glEnd();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}

void drawSquare(int image)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//add some lighting normals for each vertex
	//draw the texture on the front
	glEnable(GL_TEXTURE_2D);
	// glFrontFace(GL_CW); //use glFrontFace(GL_CW) to texture the other side - not needed here as we set this elsewhere
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);

	//bind the texture 
	glBindTexture(GL_TEXTURE_2D, textures[image]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-5.0, -5.0, 0.0);//
	glTexCoord3f(1.0, 0.0, 0.0);
	glVertex3f(5.0, -5.0, 0.0);//
	glTexCoord2f(1.0, 1.0);
	glVertex3f(5.0, 5.0, 0.0);//
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-5.0, 5.0, 0.0);//

	glEnd();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}

// Called to draw scene
void RenderScene(void)
{
	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Start with the identity matrix
	glMatrixMode(GL_MODELVIEW);

	//////////////////////////////////////////////////////////////////////
	//Textured planes for walls and floor

	glLoadIdentity();

	glTranslatef(0.0, -34.0f, -240.0f + 50.0f); //Set Pos
	CamFollow(); //Camra Follow

	drawBackWall(IMAGE1); //BACK

	glLoadIdentity();

	glTranslatef(0.0, -34.0f, -220.0f + 50.0f); //Set Pos
	CamFollow(); //Camra Follow

	drawLeftWall(IMAGE1); //LEFT

	glLoadIdentity();

	glTranslatef(0.0, -34.0f, -220.0f + 50.0f); //Set Pos
	CamFollow(); //Camra Follow

	drawRightWall(IMAGE1); //RIGHT

	glLoadIdentity();

	glTranslatef(0.0, -35.5f, -330.0f + 50.0f); //Set Pos
	CamFollow(); //Camra Follow

	drawFloor(IMAGE13); //Floor

	/////////////////////////////////////////////////////////////////////
	//Wall Thickness


	//////////////////////////////////////////////////////---Wall One

	glLoadIdentity();
	glTranslatef(0.0, 20.0f, -150.0f ); //Set Pos
	
	CamFollow(); //Camra Follow

	glTranslatef(-150.0f, 0.0f, -20.0f + 50.0f);
	glScalef(1.0f, 5.0f, 29.0f);

	glColor3ub(202, 177, 136);
	glutSolidCube(20.0f);

	//////////////////////////////////////////////////////---Wall Two

	glLoadIdentity();
	glTranslatef(0.0, 20.0f, -150.0f ); //Set Pos

	CamFollow(); //Camra Follow

	glTranslatef(150.0f, 0.0f, -20.0f + 50.0f);
	glScalef(1.0f, 5.0f, 29.0f);

	glColor3ub(202, 177, 136);
	glutSolidCube(20.0f);

	//////////////////////////////////////////////////////---Wall Three

	glLoadIdentity();
	glTranslatef(0.0, 20.0f, -150.0f + 50.0f); //Set Pos

	CamFollow(); //Camra Follow

	glTranslatef(0.0f, 0.0f, -300.0f + 0.0f);
	glScalef(15.0f, 5.0f, 1.0f);

	glColor3ub(202, 177, 136);
	glutSolidCube(20.0f);

	//////////////////////////////////////////////////////---FLOOR

	glLoadIdentity();
	glTranslatef(0.0, 20.0f, -150.0f); //Set Pos

	CamFollow(); //Camra Follow

	glTranslatef(0.0f, -66.0f, -80.0f + 50.0f);
	glScalef(20.0f, 1.0f, 30.0f);

	glColor3ub(202, 177, 136);
	glutSolidCube(20.0f);

	//////////////////////////////////////////////////////
	// Targets & Flowers with Alpha

	glLoadIdentity();

	for (int i = 0; i < Vtargets.size(); i++) //DRAW TARGETS
	{
		Vtargets[i].draw();
	}

	//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//

	glLoadIdentity();
	CamFollow(); //Camra Follow
	glTranslatef(130.0, -25.0f, -280.0f); //Set Pos
	glRotatef(20, 0.0f, 1.0f, 0.0f);
	drawPalm(IMAGE14); //Palm

	glLoadIdentity();
	CamFollow(); //Camra Follow
	glTranslatef(130.0, -25.0f, -180.0f); //Set Pos
	glRotatef(20, 0.0f, 1.0f, 0.0f);
	drawPalm(IMAGE14); //Palm

	glLoadIdentity();
	CamFollow(); //Camra Follow
	glTranslatef(130.0, -25.0f, -230.0f); //Set Pos
	glRotatef(20, 0.0f, 1.0f, 0.0f);
	drawPalm(IMAGE14); //Palm

	glLoadIdentity();
	CamFollow(); //Camra Follow
	glTranslatef(-130.0, -25.0f, -280.0f); //Set Pos
	glRotatef(20, 0.0f, 1.0f, 0.0f);
	drawPalm(IMAGE14); //Palm

	glLoadIdentity();
	CamFollow(); //Camra Follow
	glTranslatef(-130.0, -25.0f, -180.0f); //Set Pos
	glRotatef(20, 0.0f, 1.0f, 0.0f);
	drawPalm(IMAGE14); //Palm

	glLoadIdentity();
	CamFollow(); //Camra Follow
	glTranslatef(-130.0, -25.0f, -230.0f); //Set Pos
	glRotatef(20, 0.0f, 1.0f, 0.0f);
	drawPalm(IMAGE14); //Palm

	//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//

	glLoadIdentity();
	glTranslatef(80.0, -25.0f, -180.0f); //Set Pos
	CamFollow(); //Camra Follow
	drawFlower(IMAGE11); //Flower1

	glLoadIdentity();
	glTranslatef(-80.0, -25.0f, -180.0f); //Set Pos
	CamFollow(); //Camra Follow
	drawFlower(IMAGE12); //Flower2

	glLoadIdentity();
	glTranslatef(50.0, -25.0f, -240.0f); //Set Pos
	CamFollow(); //Camra Follow
	drawFlower(IMAGE12); //Flower3

	glLoadIdentity();
	glTranslatef(-50.0, -25.0f, -240.0f); //Set Pos
	CamFollow(); //Camra Follow
	drawFlower(IMAGE11); //Flower3

	///////////////////////////////////////////////////////////
   //Spot Light 

	glEnable(GL_LIGHTING);

	glLoadIdentity();
	CamFollow(); //Camra Follow
	//glTranslatef(0.0, 20.0f, -150.0f - 50.0f);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specref);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, mKd);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 120.0f);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, lightDir);
	glEnable(GL_LIGHT1);

	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specref);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, mKd);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteLight);
	glEnable(GL_LIGHT0);

	//glDisable(GL_LIGHTING);

	//glMaterialf(GL_FRONT, GL_SHININESS, shine);

	///////////////////////////////////////////////////////////////////
	// BALL

	glLoadIdentity();
	// Translate the whole scene out and into view
	glTranslatef(sunX, sunY, sunZ + 25.0f);
	CamFollow(); //Camra Follow
	if (move)
	{
		glRotatef(fMoonRot, 1.0f, 0.0f, 0.0f);
		glTranslatef(0, 0, 0);
	}
	
	// Set material colour, yellow Sun
	//glDisable(GL_LIGHTING);

	glEnable(GL_DEPTH_TEST);

	GLUquadricObj *quadric;
	quadric = gluNewQuadric();

	glEnable(GL_TEXTURE_2D);
	//glColor3ub(225, 0, 225);
	gluQuadricDrawStyle(quadric, GLU_FILL);

	glBindTexture(GL_TEXTURE_2D, IMAGE4);
	gluQuadricTexture(quadric, GL_TRUE);
	gluQuadricNormals(quadric, GLU_SMOOTH);

	gluSphere(quadric, 15.0f, 30, 17); //SUN

	glDisable(GL_TEXTURE_2D);

	glPushMatrix();

	glLoadIdentity();
	CamFollow(); //Camra Follow
	glTranslatef(0.0f, -20.0f - 12.0f, -150.0f + 20.0f);
	glRotatef(-80, 1.0f, 0.0f, 0.0f);
	drawSquare(IMAGE6); //Palm

	///////////////////////////////////////////////////////////
	//SkyBox
	glDisable(GL_LIGHTING);
	glLoadIdentity();

	glTranslatef(0.0, -10.0f, -100.0f); //Set Pos
	glScalef(322.0f, 322.0f, 322.0f);
	glRotatef(fEarthRot, 0.0f, 1.0f, 0.0f);
	//CamFollow(); //Camra Follow

	drawSkyBox(IMAGE7); //SKY BOX
	drawHUD(); //Draw UI

	glEnable(GL_LIGHTING);

	glutSwapBuffers();
}

void SetupRC()
{
	//textures

	GLuint texture;
	// allocate a texture name
	glGenTextures(1, &texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	// photoshop is a good converter to targa (TGA)
	//the gltLoadTGA method is found in gltools.cpp and is orignally from the OpenGL SuperBible book
	//there are quite a few ways of loading images
	// Load textures in a for loop
	glGenTextures(TEXTURE_COUNT, textures);
	//this puts the texture into OpenGL texture memory
 //   glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); - not defined so probably need to update GLEW - not needed here so ignore
	for (int iLoop = 0; iLoop < TEXTURE_COUNT; iLoop++)
	{
		// Bind to next texture object
		glBindTexture(GL_TEXTURE_2D, textures[iLoop]);

		// Load texture data, set filter and wrap modes
		//note that gltLoadTGA is in the glm.cpp file and not a built-in openGL function
		pBytes0 = gltLoadTGA(textureFiles[iLoop], &iWidth, &iHeight,
			&iComponents, &eFormat);

		glTexImage2D(GL_TEXTURE_2D, 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBytes0);

		//set up texture parameters

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		//try these too
	   // glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);
		// glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		free(pBytes0);
	}

	//enable textures
	glEnable(GL_TEXTURE_2D);


	glEnable(GL_DEPTH_TEST);	// Hidden surface removal    

	glFrontFace(GL_CCW);// Counter clock-wise polygons face out

	glEnable(GL_CULL_FACE);		// Do not calculate inside

//    glCullFace(GL_FRONT_AND_BACK);

	glEnable(GL_LIGHTING);
	glEnable(GL_POINT_SMOOTH);
	// Setup and enable light 0
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sourceLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, mKs);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glEnable(GL_LIGHT0);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteLight);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, sourceLight);
	glLightfv(GL_LIGHT1, GL_SPECULAR, mKs);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos);
	glEnable(GL_LIGHT1);

	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 50.0f);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 50.0f);
	// Enable color tracking
	glEnable(GL_COLOR_MATERIAL);

	// Set Material properties to follow glColor values
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	// Enable colour tracking
	glEnable(GL_COLOR_MATERIAL);

	// Set Material properties to follow glColor values
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
	glMateriali(GL_FRONT, GL_SHININESS, 128);

	// Black blue background
	glClearColor(0.0f, 0.0f, 0.03f, 0.0f);
}

void KeyBoard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'f': 
		if (!gameOver)
		{
			if (follow)
			{
				follow = false;
			}
			else
			{
				follow = true;
			}
		}
		break;
	case 'r':

		for (int i = 0; i < Vtargets.size(); i++) //DRAW TARGETS
		{
			Vtargets[i].hit = false;
		}

		score = 0;
		sunMov_X = 0.0f; // 1 - 9
		sunMov_Y = 1.0f; // 3 - 9
		sunMov_Yy = 0.0f;

		follow = false;
		gameOver = false;
		counter = 0;
		hitCount =0;

		move = false;
		sunX = 0.0f;
		sunY = -20.0f;
		sunZ = -150.0f;

		break;
	case 27:
		glutDestroyWindow(window);
		exit(0);
		break;
	case 32:
		
		if(!gameOver)
		if (!move)
		{
			move = true;
			sunMov_Yy = sunMov_Y;
			counter++;
		}

		break;
	}

	glutPostRedisplay(); //Refresh
}

void SpecialInput(int key, int x, int y) //KEYBOARD INPUT MOVEMENT
{
	if (!move)
	{
		switch (key)
		{
		case GLUT_KEY_UP:

			if (!gameOver)
			if (sunMov_Y < 9.0)
				sunMov_Y += 1.0f; // 3 - 9

			break;
		case GLUT_KEY_DOWN:

			if (!gameOver)
			if (sunMov_Y > 1.0)
				sunMov_Y -= 1.0f; // 3 - 9

			break;
		case GLUT_KEY_LEFT:

			if (!gameOver)
			if (sunMov_X > -9.0)
				sunMov_X -= 1.0f;

			break;
		case GLUT_KEY_RIGHT:

			if (!gameOver)
			if (sunMov_X < 9.0)
				sunMov_X += 1.0f;

			break;
		}
	}

	glutPostRedisplay(); //Refresh
}

void TimerFunc(int value)
{

	for (int i = 0; i < Vtargets.size(); i++) //Collide and Erase
	{
		Vtargets[i].collision();
			//Vtargets.erase(Vtargets.begin() + i);
	}

	fEarthRot += 0.05f;
	if (fEarthRot > 360.0f)
	{
		fEarthRot = 0.0f;
	}

	if (move)
	{
		sunX += sunMov_X;

		if(sunY >= -20)
			sunY += sunMov_Yy;

		sunMov_Yy -= 0.25f;

		if (sunZ >= -600.0f)
		{
			sunZ -= spd;
		}
		else
		{
			move = false;
			sunX = 0.0f;
			sunY = -20.0f;
			sunZ = -150.0f;

			if (counter >= 10 || hitCount >= 7)
				gameOver = true;
		}
	}

	// Step Moon orbit 4 degrees
	fMoonRot += 4.0f;
	if (fMoonRot > 360.0f)
	{
		fMoonRot = 0.0f;
	}

	glutPostRedisplay();
	glutTimerFunc(25, TimerFunc, 1);
}

void ChangeSize(int w, int h)
{
	GLfloat fAspect;

	// Prevent a divide by zero
	if (h == 0)
		h = 1;

	// Set Viewport to window dimensions
	glViewport(0, 0, w, h);

	// Calculate aspect ratio of the window
	fAspect = (GLfloat)w / (GLfloat)h;

	// Set the perspective coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// field of view of 45 degrees, near and far planes 1.0 and 425
	gluPerspective(45.0f, fAspect, 1.0, 600.0);
	// Modelview matrix reset
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1066, 600);// a 16:9 ratio

	int window;
	window = glutCreateWindow("CW2 - Joseph Calabro - W1635359");

	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);
	glutKeyboardFunc(KeyBoard);
	glutSpecialFunc(SpecialInput);
	glutTimerFunc(25, TimerFunc, 1);
	SetupRC();
	createTargets();
	glutMainLoop();
	return 0;
}
//
//  main.cpp
//  OpenGL_2
//
//  Created by 朱坤 on 16/1/13.
//  Copyright © 2016年 zhukun. All rights reserved.
//

#if defined(_WIN32)
#include "glut.h"
#include <windows.h>
#else
#include <GLUT/glut.h>
#endif

#include <math.h>
#include "RGBpixmap.h"
#include "SolarSystemGlobals.h"


// Viewer positioning angles.
GLfloat viewerAzimuth = INITIAL_VIEWER_AZIMUTH;
GLfloat viewerZenith  = INITIAL_VIEWER_ZENITH;

// ID numbers for all texture maps.
//GLuint ParticleTextureName = -1;
GLuint EarthTextureName = 3;
GLuint MoonTextureName  = 4;
GLuint SunTextureName   = 0;


// Variables to keep track of current day status.
GLfloat CurrentEarthRotation = 0.00;
GLfloat EarthDaysTranspired  = 0.00;
GLfloat EarthDayIncrement    = 0.01;

// The initial window and viewport sizes (in pixels), set to ensure that
// the aspect ration for the viewport, will be a constant. If the window
// is resized, the viewport will be adjusted to preserve the aspect ratio.
GLint currWindowSize[2]   = { 800, 600 };
GLint currViewportSize[2] = { 800, 600 };

GLfloat ViewerDistance = INITIAL_VIEWER_DISTANCE;

//ParticleSystem particles;
//bool particleFlag = true;
bool blendFlag = true;

void TimerFunction(int value);
void Display();

void MakeAllImages();
void MakeImage(const char bitmapFilename[], GLuint &textureName, bool hasAlpha);

void SetLights();
void UpdateLight();

void ResizeWindow(GLsizei w, GLsizei h);

void drawEarthAndMoon();
void drawSun();
//void drawSaturnRing();
void drawAllPlanets();
void drawGenericPlanet(GLfloat inclination, GLfloat orbitDuration,
                       GLfloat orbitRadius, GLfloat rotationDuration, GLuint texturename, GLfloat radius);
// The main function sets up the data and the   //
// environment to display the textured objects. //
int main(int argc, char** argv)
{
    glutInit (&argc, argv);//初始化
    
    // Set up the display window.
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_STENCIL | GLUT_DEPTH );//设定显示模式
    glutInitWindowPosition( INIT_WINDOW_POSITION[0], INIT_WINDOW_POSITION[1] );//位置
    glutInitWindowSize( currWindowSize[0], currWindowSize[1] );//大小
    glutCreateWindow( "太阳系");//窗口名称
    
    // Specify the resizing and refreshing routines.
    glutReshapeFunc( ResizeWindow );
    glutDisplayFunc( Display );
    glutTimerFunc( 20, TimerFunction, 1 );
    glViewport(0, 0, currWindowSize[0], currWindowSize[1]);
    
    // Set up standard lighting, shading, and depth testing.
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
    glEnable(GL_NORMALIZE);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    SetLights();
    
    // Set up all texture maps and texture-mapped objects.
    MakeAllImages();
    glutMainLoop();
    return 0;
}

// Function to react to ASCII keyboard keys pressed by the user.   //
// +/- keys are used to accelerate/decelerate the animation, while //
// the z/Z keys are used to zoom in and out of the animation.      //
void KeyboardPress(unsigned char pressedKey, int mouseXPosition, int mouseYPosition)
{
}

// Function to react to non-ASCII keyboard keys pressed by the user. //
// Used to alter spherical coordinates of the viewer's position.     //
void NonASCIIKeyboardPress(int pressedKey, int mouseXPosition, int mouseYPosition)
{
    glutIgnoreKeyRepeat(false);
    switch(pressedKey)
    {
        case GLUT_KEY_RIGHT: {
            viewerAzimuth += VIEWER_ANGLE_INCREMENT;
            if (viewerAzimuth > 2*PI)
                viewerAzimuth -= 2*PI;
            break;
							 }
        case GLUT_KEY_LEFT:  {
            viewerAzimuth -= VIEWER_ANGLE_INCREMENT;
            if (viewerAzimuth < 0.0)
                viewerAzimuth += 2*PI;
            break;
							 }
        case GLUT_KEY_UP:    {
            viewerZenith -= VIEWER_ANGLE_INCREMENT;
            if (viewerZenith < VIEWER_ANGLE_INCREMENT)
                viewerZenith = VIEWER_ANGLE_INCREMENT;
            break;
							 }
        case GLUT_KEY_DOWN:  {
            viewerZenith += VIEWER_ANGLE_INCREMENT;
            if (viewerZenith > PI - VIEWER_ANGLE_INCREMENT)
                viewerZenith = PI - VIEWER_ANGLE_INCREMENT;
            break;
							 }
    }
}

// The EarthDayIncrement represents the fraction of an      //
// Earth day being added to the scene in one screen update. //
void TimerFunction(int value)
{
    CurrentEarthRotation += EarthDayIncrement;
    EarthDaysTranspired += EarthDayIncrement;
    if (EarthDaysTranspired == EARTH_ORBIT_DUR)
        EarthDaysTranspired = 0;
    glutPostRedisplay();
    glutTimerFunc(20, TimerFunction, 1);
}

// Principal display routine: sets up material, lighting, //
// and camera properties, clears the frame buffer, and    //
// draws all texture-mapped objects within the window.    //
void Display()
{
    // Initialize lighting.
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LIGHT_MODEL_AMBIENT);
    glEnable(GL_LIGHTING);
    
    // Set up the properties of the viewing camera.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, ASPECT_RATIO, 0.2, 100.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT| GL_STENCIL_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Position and orient viewer.
    gluLookAt(LOOK_AT_POSITION[0] + ViewerDistance * sin(viewerZenith) * sin(viewerAzimuth),
              LOOK_AT_POSITION[1] + ViewerDistance * cos(viewerZenith),
              LOOK_AT_POSITION[2] + ViewerDistance * sin(viewerZenith) * cos(viewerAzimuth),
              LOOK_AT_POSITION[0], LOOK_AT_POSITION[1], LOOK_AT_POSITION[2],
              0.0, 1.0, 0.020);
    
    // Render scene.
    UpdateLight();
    drawEarthAndMoon();
    drawSun();
    
    if(blendFlag){ //if the user has selected blending, enable it.
        glEnable(GL_BLEND);
        glDepthMask(GL_FALSE);
        glBlendFunc(GL_SRC_COLOR,GL_ONE);
    }
    
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glDisable(GL_LIGHTING);
    glutSwapBuffers();
    glFlush();
}

// Create the textures associated with all texture-mapped objects being displayed. //
void MakeAllImages()
{
    MakeImage(EARTH_BMP_FILENAME, EarthTextureName, false);
    MakeImage(MOON_BMP_FILENAME,  MoonTextureName,  false);
    MakeImage(SUN_BMP_FILENAME,		SunTextureName,		false);
    return;
}


// Convert the bitmap with the parameterized name into an OpenGL texture. //
void MakeImage(const char bitmapFilename[], GLuint &textureName, bool hasAlpha)
{
    RGBpixmap pix;
    pix.readBMPFile(bitmapFilename, hasAlpha);
    pix.setTexture(textureName);
    return;
}


// Set the two lights to illuminate the scene. //
void SetLights()
{
    glLightfv(GL_LIGHT0, GL_AMBIENT,  LIGHT_AMBIENT);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  LIGHT_DIFFUSE);
    glLightfv(GL_LIGHT0, GL_SPECULAR, LIGHT_SPECULAR);
    glLightfv(GL_LIGHT0, GL_POSITION, LIGHT_0_POSITION);
    
    glLightfv(GL_LIGHT1, GL_AMBIENT,  LIGHT_AMBIENT);
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  LIGHT_DIFFUSE);
    glLightfv(GL_LIGHT1, GL_SPECULAR, LIGHT_SPECULAR);
    glLightfv(GL_LIGHT1, GL_POSITION, LIGHT_1_POSITION);
}


// Enable the scene's lighting. //
void UpdateLight()
{
    glPushMatrix();
    glLightfv(GL_LIGHT0, GL_POSITION, LIGHT_0_POSITION);
    glLightfv(GL_LIGHT1, GL_POSITION, LIGHT_1_POSITION);
    glLightfv(GL_LIGHT2, GL_POSITION, LIGHT_2_POSITION);
    glLightfv(GL_LIGHT3, GL_POSITION, LIGHT_3_POSITION);
    glPopMatrix();
    
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
}

// Window-reshaping callback, adjusting the viewport to be as large  //
// as possible within the window, without changing its aspect ratio. //
void ResizeWindow(GLsizei w, GLsizei h)
{
    currWindowSize[0] = w;
    currWindowSize[1] = h;
    if (ASPECT_RATIO > w/h)
    {
        currViewportSize[0] = w;
        currViewportSize[1] = w / ASPECT_RATIO;
    }
    else
    {
        currViewportSize[1] = h;
        currViewportSize[0] = h * ASPECT_RATIO;
    }
    
    glViewport(0.5*(w-currViewportSize[0]), 0.5*(h-currViewportSize[1]), currViewportSize[0], currViewportSize[1]);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glTranslatef(0.0, 0.0, -20.0);
    gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


//Draws the texture-mapped earth and moon. //
void drawEarthAndMoon()
{
    glDisable(GL_LIGHT1);
    
    GLfloat MoonRevolution = EarthDaysTranspired / LUNAR_CYCLE;
    GLUquadricObj* quadro = gluNewQuadric();
    gluQuadricNormals(quadro, GLU_SMOOTH);
    gluQuadricTexture(quadro, GL_TRUE);
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glPushMatrix();
				glRotatef(EARTH_INCLINATION, 0.0, 0.0, 0.0);
				glRotatef( 360.0 * (EarthDaysTranspired/EARTH_ORBIT_DUR), 0.0, 1.0, 0.0);
				glTranslatef(EARTH_ORBIT_RADIUS, 0.0, 0.0 );
				glRotatef( 360.0 * CurrentEarthRotation, 0.0, 1.0, 0.0 );
				glRotatef( -90.0, 1.0, 0.0, 0.0 );
				glBindTexture(GL_TEXTURE_2D, EarthTextureName);
				gluSphere(quadro, EARTH_RADIUS, 48, 48);
    glPopMatrix();
    glRotatef(EARTH_INCLINATION, 0.0, 0.0, 0.0);
    glRotatef( 360.0 * (EarthDaysTranspired/EARTH_ORBIT_DUR), 0.0, 1.0, 0.0);
    glTranslatef(EARTH_ORBIT_RADIUS, 0.0, 0.0 );
    glRotatef( 360.0 * MoonRevolution, 0.0, 1.0, 0.0 );
    glTranslatef( MOON_ORBIT_RADIUS  , 0.0, 0.0 );
    glBindTexture(GL_TEXTURE_2D, MoonTextureName);
    gluSphere(quadro, MOON_RADIUS, 48, 48);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    gluDeleteQuadric(quadro);
    
    glEnable(GL_LIGHT1);
}

//Function to draw and texture map the sun at the origin
void drawSun()
{
    glDisable(GL_LIGHT0);
    
    GLUquadricObj* quadro = gluNewQuadric();
    gluQuadricNormals(quadro, GLU_SMOOTH);
    gluQuadricTexture(quadro, GL_TRUE);
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glPushMatrix();
    
    glRotatef(EARTH_INCLINATION, 0.0, 0.0, 0.0);
    glRotatef( 360.0 * (EarthDaysTranspired/EARTH_ORBIT_DUR), 0.0, 1.0, 0.0);
    glRotatef( 360.0 * 0, 0.0, 1.0, 0.0 );
    
				//glRotatef( -90.0, 1.0, 0.0, 0.0 );
				glBindTexture(GL_TEXTURE_2D, SunTextureName);
				gluSphere(quadro, SUN_RADIUS, 48, 48);
    glPopMatrix();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    gluDeleteQuadric(quadro);
    
    glEnable(GL_LIGHT0);
}

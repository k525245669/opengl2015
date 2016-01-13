/////////////////////////////////////////////////////////
// SolarSystemGlobals.h - Global Constant Definitions //
/////////////////////////////////////////////////////////

#ifndef SOLAR_SYSTEM_GLOBALS_H 

/********************/
/* Global Constants */
/********************/

const GLfloat PI = 3.1415926535;
const GLfloat TO_RADIANS = PI/180.0;

// Window Position/Resizing Constants
const GLint INIT_WINDOW_POSITION[2] = { 150, 150 };
const GLfloat ASPECT_RATIO = 1.5;

// Lighting Constants
const GLfloat LIGHT_0_POSITION[]    = { 0.0, 0.0, 0.0, 1.0};
const GLfloat LIGHT_1_POSITION[]    = { 0.0, 0.0, 1.0, 0.0};
const GLfloat LIGHT_2_POSITION[]    = { 0.0, 0.0, 1.0, 0.0};
const GLfloat LIGHT_3_POSITION[]    = { 0.0, 0.0, -1.0, 0.0};
const GLfloat LIGHT_AMBIENT[]       = { 0.8, 0.8, 0.8, 1.0};
const GLfloat LIGHT_DIFFUSE[]       = { 0.9, 0.9, 0.9, 1.0};
const GLfloat LIGHT_SPECULAR[]      = { 1.0, 1.0, 1.0, 1.0};
const GLfloat LIGHT_MODEL_AMBIENT[] = { 0.2, 0.2, 0.2, 1.0};

// Earth and Moon Constants
const char EARTH_BMP_FILENAME[] = "/Users/zhukkun/Documents/workspace/opengl2015/21551012朱坤坤/OpenGL_2/OpenGL_2/earthmap.bmp";
const char MOON_BMP_FILENAME[]  = "/Users/zhukkun/Documents/workspace/opengl2015/21551012朱坤坤/OpenGL_2/OpenGL_2/moonmap.bmp";
const GLfloat EARTH_RADIUS      = 0.54;
const GLfloat MOON_RADIUS       = 0.12;
const GLfloat MOON_ORBIT_RADIUS = 0.90;
const GLfloat EARTH_ROTATION    = 1.00;
const GLfloat LUNAR_CYCLE       = 5.0;

// Viewer Positioning Constants
const GLfloat MAXIMUM_VIEWER_DISTANCE    = 20;
const GLfloat MINIMUM_VIEWER_DISTANCE    = 2;
const GLfloat INITIAL_VIEWER_DISTANCE    = 5;
const GLfloat VIEWER_DISTANCE_INCREMENT  = 0.1;
const GLfloat INITIAL_VIEWER_AZIMUTH     = 0.0;
const GLfloat INITIAL_VIEWER_ZENITH      = PI / 2.0;
const GLfloat VIEWER_ANGLE_INCREMENT     = PI / 1.0;
const GLfloat LOOK_AT_POSITION[] = { 0.0, 0.0, 0.0 };

const char SUN_BMP_FILENAME[] = "/Users/zhukkun/Documents/workspace/opengl2015/21551012朱坤坤/OpenGL_2/OpenGL_2/sunmap.bmp";

const GLfloat SUN_RADIUS = 			1.0;//太阳半径

const GLfloat EARTH_ORBIT_RADIUS = 	 3.0;//地球公转半径

const GLfloat EARTH_ORBIT_DUR=		36;//地球一年日期

const GLfloat EARTH_INCLINATION   = 7;


#define SOLAR_SYSTEM_H
#endif
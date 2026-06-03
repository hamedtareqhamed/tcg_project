#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <GL/glut.h>
#include "battleArena.hpp"
#include "main.hpp"
#include "charizardy.hpp"
#include "MyDetailedBlastoise.hpp"
//#include "Project.hpp"

//using Project::MyVirtualWorld;

// Global instances of the virtual worlds
charizardx::MyVirtualWorld myvirtualworld_charizard;
BlastoiseLab::MyVirtualWorld myvirtualworld_blastoise;
battleArena::MyVirtualWorld myvirtualworld_arena;
using namespace std;

// Global configuration variables defined in main.hpp
MyWindow   window;
MyWorld    world;
MyViewer   viewer;
MySetting  setting;
MyAxis     worldaxis;

// Core display callback, invoked every frame
void myDisplayFunc(void)
{
 // Clear color and depth buffers
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

 glPushMatrix();

    // Apply global world transformations (camera panning/rotating)
    glTranslatef(world.posX, world.posY, world.posZ);
    glRotatef(world.rotateX, 1.0f, 0.0f, 0.0f);
    glRotatef(world.rotateY, 0.0f, 1.0f, 0.0f);
    glRotatef(world.rotateZ, 0.0f, 0.0f, 1.0f);
    glScalef(world.scaleX, world.scaleY, world.scaleZ);

    // Render the XYZ reference axis
    worldaxis.draw();
    glPushMatrix();
    glTranslatef(-60.0f, 20.0f, 0.0f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    myvirtualworld_charizard.draw();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glScalef(2.0f, 2.0f, 2.0f);
    myvirtualworld_arena.draw();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(60.0f, 20.0f, 0.0f);
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    myvirtualworld_blastoise.draw();
    glPopMatrix();

 glPopMatrix();

 // Force execution of GL commands and swap the double buffers
 glFlush();
 glutSwapBuffers();

 // Update animation timers and request a screen redraw for continuous animation
 myvirtualworld_charizard.tickTime();
 myvirtualworld_blastoise.tickTime();
 myvirtualworld_arena.tickTime();
 glutPostRedisplay();
}

// Window resize callback to maintain aspect ratio
void myReshapeFunc(int width, int height)
{
 window.width  = width;
 window.height = height;
 glViewport(0, 0, width, height); // Map OpenGL coordinates to the entire window
}

// Standard keyboard input callback (Letters and numbers)
void myKeyboardFunc(unsigned char key, int x, int y)
{
 GLfloat xinc, yinc, zinc;
 xinc = yinc = zinc = 0.0;

 switch (key)
 {
    case 'a': case 'A': xinc = -setting.posInc;  break;
    case 'd': case 'D': xinc =  setting.posInc;  break;
    case 'q': case 'Q': yinc = -setting.posInc;  break;
    case 'e': case 'E': yinc =  setting.posInc;  break;
    case 'w': case 'W': zinc = -setting.posInc;  break;
    case 's': case 'S': zinc =  setting.posInc;  break;

    //case 'o': case 'O': myvirtualworld.triggerBoxOpen(); break;

    //case 'c': case 'C': myvirtualworld.charizard.castSkill(); break;

    // Toggle animations for Charizard X and Blastoise
    case 'r': case 'R': myvirtualworld_charizard.toggleWalking(); break;
    case 'f': case 'F': myvirtualworld_charizard.toggleFireBreath(); break;
    case 'g': case 'G': myvirtualworld_charizard.toggleFlying(); break;
    case 'o': myvirtualworld_charizard.triggerBoxOpen(); break;
    case 'O': myvirtualworld_blastoise.triggerBoxOpen(); break;
    case 'c': case 'C': myvirtualworld_blastoise.castBlastoiseSkill(); break;
    case 'b': case 'B': myvirtualworld_blastoise.toggleBattlePhase(); break;
    case 'v': {
        bool current = myvirtualworld_charizard.isEnvironmentActive();
        myvirtualworld_charizard.setEnvironmentActive(!current);
        break;
    }
    case 'V':{
        bool current = myvirtualworld_blastoise.isEnvironmentActive();
        myvirtualworld_blastoise.setEnvironmentActive(!current);
        break;
    }

    case 27  : exit(1); break; // ESC to exit
 }

 // Apply movement to the global world matrix
 world.move(xinc, yinc, zinc);

 glutPostRedisplay();
}

// Special keyboard input callback (Arrow keys, F1-F12)
void mySpecialFunc(int key, int x, int y)
{
 switch (key)
 {
    // Arrow keys to rotate the scene
    case GLUT_KEY_DOWN  : world.rotateX -= setting.angleInc;  break;
    case GLUT_KEY_UP    : world.rotateX += setting.angleInc;  break;
    case GLUT_KEY_LEFT  : world.rotateY -= setting.angleInc;  break;
    case GLUT_KEY_RIGHT : world.rotateY += setting.angleInc;  break;

    case GLUT_KEY_HOME  : myDataInit(); break; // Reset configurations

    // Toggle Polygon rendering mode (Solid vs Wireframe)
 	case GLUT_KEY_F1    : setting.shadingMode = !setting.shadingMode;
                          if (setting.shadingMode)
                          	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                          else
	                        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                          break;

    // Toggle Axis visibility
	case GLUT_KEY_F2    : worldaxis.toggle(); break;

	// Toggle global lighting
	case GLUT_KEY_F3    : GLboolean lightingIsOn;
                          glGetBooleanv(GL_LIGHTING, &lightingIsOn);
                          if (lightingIsOn == GL_TRUE)
                             glDisable(GL_LIGHTING);
                          else
                             glEnable(GL_LIGHTING);
                          break;
 }
 glutPostRedisplay();
}

// Mouse click callback
void myMouseFunc(int button, int state, int x, int y)
{
 y = window.height - y; // Invert Y axis
 switch (button)
 {
    case GLUT_RIGHT_BUTTON:
       if (state == GLUT_DOWN && !setting.mouseRightMode)
       {
          setting.mouseX = x;
          setting.mouseY = y;
          setting.mouseRightMode = true;
       }
       if (state == GLUT_UP && setting.mouseRightMode)
       {
          setting.mouseRightMode = false;
       }
       break;
    case GLUT_LEFT_BUTTON:
       if (state == GLUT_DOWN && !setting.mouseLeftMode)
       {
          setting.mouseX = x;
          setting.mouseY = y;
          setting.mouseLeftMode = true;
       }
       if (state == GLUT_UP && setting.mouseLeftMode)
       {
          setting.mouseLeftMode = false;
       }
       break;
 }
}

// Mouse movement callback while holding down buttons
void myMotionFunc(int x, int y)
{
 y = window.height - y;
 GLint xinc = x - setting.mouseX;
 GLint yinc = y - setting.mouseY;

 // Rotate scene using Right Click drag
 if(setting.mouseRightMode)
 {
    world.rotate(0.0f, 0.0f, -xinc * 0.5);
 }
 // Rotate scene using Left Click drag
 if(setting.mouseLeftMode)
 {
    world.rotate(-yinc * 0.5, xinc * 0.5, 0.0f);
 }

 setting.mouseX = x;
 setting.mouseY = y;
 glutPostRedisplay();
}

// Initializes application data structures
void myDataInit()
{
 window.title = "TCG6223 Computer Graphics";
 window.posX = 100;
 window.posY = 100;
 window.width  = 800;
 window.height = 500;

 world.rotateX  = 0.0;
 world.rotateY  = 0.0;
 world.rotateZ  = 0.0;
 world.posX     = 0.0;
 world.posY     = 0.0;
 world.posZ     = 0.0;
 world.scaleX   = 1.0;
 world.scaleY   = 1.0;
 world.scaleZ   = 1.0;

 // Setup camera position (slightly pulled back on Z-axis)
 viewer.eyeX    = 0.0;
 viewer.eyeY    = 0.0;
 viewer.eyeZ    = 150.0;
 viewer.centerX = 0.0;
 viewer.centerY = 0.0;
 viewer.centerZ = 0.0;
 viewer.upX     = 0.0;
 viewer.upY     = 1.0;
 viewer.upZ     = 0.0;
 viewer.zNear   = 0.1;
 viewer.zFar    = 1500.0;
 viewer.fieldOfView = 60.0;
 viewer.aspectRatio = static_cast<GLdouble> (window.width) / window.height;

 setting.posInc   = 1.0;
 setting.angleInc = 2.0;
 setting.mouseX   = 0;
 setting.mouseY   = 0;

 setting.mouseRightMode = false;
 setting.mouseLeftMode = false;

 setting.shadingMode = true;
}

// Configures the Projection and ModelView matrices
void myViewingInit()
{
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 gluPerspective(viewer.fieldOfView,
                viewer.aspectRatio,
                viewer.zNear,
                viewer.zFar);

 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
 gluLookAt(viewer.eyeX,   viewer.eyeY,   viewer.eyeZ,
           viewer.centerX,viewer.centerY,viewer.centerZ,
           viewer.upX,    viewer.upY,    viewer.upZ );
}

// Initializes a basic global light source (LIGHT0)
void myLightingInit()
{
 static GLfloat  ambient[] = { 0.0f,  0.0f,  0.0f, 1.0f };
 static GLfloat  diffuse[] = { 1.0f,  1.0f,  1.0f, 1.0f };
 static GLfloat specular[] = { 1.0f,  1.0f,  1.0f, 1.0f };
 static GLfloat  specref[] = { 1.0f,  1.0f,  1.0f, 1.0f };
 static GLfloat position[] = {10.0f, 10.0f, 10.0f, 0.0f }; // Overhead light source
 short shininess = 128;

 glEnable(GL_LIGHTING);
 glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
 glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
 glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
 glLightfv(GL_LIGHT0, GL_POSITION, position);
 glEnable(GL_LIGHT0);

 // Enables tracking of object colors against the lighting
 glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
 glEnable(GL_COLOR_MATERIAL);

 glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
 glMateriali(GL_FRONT, GL_SHININESS, shininess);

 // Normalizes vectors to prevent lighting distortion from glScalef()
 glEnable(GL_NORMALIZE);
}

// Master initialization routine
void myInit()
{
 myDataInit();

 // Initialize GLUT display mode
 glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
 glutInitWindowPosition(window.posX, window.posY);
 glutInitWindowSize(window.width, window.height);
 glutCreateWindow(window.title.c_str());

 // Bind callbacks
 glutDisplayFunc(myDisplayFunc);
 glutReshapeFunc(myReshapeFunc);
 glutKeyboardFunc(myKeyboardFunc);
 glutSpecialFunc(mySpecialFunc);
 glutMotionFunc(myMotionFunc);
 glutMouseFunc(myMouseFunc);

 // OpenGL Environment settings
 glPointSize(4.0);
 glEnable(GL_DEPTH_TEST);
 glDepthFunc(GL_LESS);
 glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
 glFrontFace(GL_CCW);
 glShadeModel(GL_SMOOTH); // Gouraud shading
 glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Black background
 glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
 glEnable(GL_CULL_FACE); // Optimize by culling backfaces

 myViewingInit();
 myLightingInit();

 // Initialize the models
 myvirtualworld_charizard.init();
 myvirtualworld_blastoise.init();
 myvirtualworld_arena.init();

 // Turn off character environments by default
 myvirtualworld_charizard.setEnvironmentActive(false);
 myvirtualworld_blastoise.setEnvironmentActive(false);
}

// Print control instructions to the console
void myWelcome()
{
 cout << "*****************************************************************\n";
 cout << "* TCG6223 Computer Graphics                                     *\n";
 cout << "* FIST, Multimedia University                                   *\n";
 cout << "*****************************************************************\n";
 cout << "| Press:                                                        |\n";
 cout << "|   <a>,<d>,<w>,<s>,<q>,<e> => move world                       |\n";
 cout << "|   <r>                    => toggle Charizard X walking        |\n";
 cout << "|   <arrows>                => rotate world                     |\n";
 cout << "|   HOME                    => restore defaults                 |\n";
 cout << "|   ESC                     => exit                             |\n";
 cout << "|                                                               |\n";
 cout << "|   F1                      => toggle shading / wire-frame mode |\n";
 cout << "|   F2                      => toggle rendering of axes         |\n";
 cout << "|   F3                      => toggle lighting on / off         |\n";
 cout << "|   <v> <V>                    => toggle character environments     |\n";
 cout << "|                                                               |\n";
 cout << "| Mouse (Left Drag or Right Drag) => rotate world               |\n";
 cout << "| Character Actions:                                            |\n";
 cout << "|   <o> <O>                => Open Blind Box (Reveal!)          |\n";
 cout << "|   <r>                    => toggle Charizard X walking        |\n";
 cout << "|   <g>                    => toggle Charizard X flying         |\n";
 cout << "|   <f>                    => trigger Blue Flare (Dragon Breath)|\n";
 cout << "|                                                               |\n";
 cout << "*****************************************************************\n";
 cout << "|                      H A V E   F U N  !!!                     |\n";
 cout << "*****************************************************************\n";
}

int main(int argc, char **argv)
{
 glutInit(&argc, argv);

 myWelcome();

 myInit();

 // Enter the infinite GLUT event processing loop
 glutMainLoop();
}

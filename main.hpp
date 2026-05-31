#ifndef YP_CGLABMAIN_HPP
#define YP_CGLABMAIN_HPP

#include <string>
#include <cmath>
#include <iostream>
#include <GL/glut.h>

using namespace std;

// Function prototypes for GLUT callback routines
void myDisplayFunc(void);
void myReshapeFunc(int width, int height);
void myKeyboardFunc(unsigned char key, int x, int y);
void mySpecialFunc(int key, int x, int y);
void myMouseFunc(int button, int state, int x, int y);
void myMotionFunc(int x, int y);
void myTimerFunc(int value);

// Application initialization routines
void myDataInit();
void myViewingInit();
void myInit();
void myWelcome();
void myLightingInit();

// Pre-defined static colors for general usage
static GLfloat myblack[]   = { 0.0f, 0.0f, 0.0f, 1.0f };
static GLfloat mywhite[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat myred[]     = { 1.0f, 0.0f, 0.0f, 1.0f };
static GLfloat mygreen[]   = { 0.0f, 1.0f, 0.0f, 1.0f };
static GLfloat myblue[]    = { 0.0f, 0.0f, 1.0f, 1.0f };
static GLfloat mycyan[]    = { 0.0f, 1.0f, 1.0f, 1.0f };
static GLfloat mymagenta[] = { 1.0f, 0.0f, 1.0f, 1.0f };
static GLfloat myyellow[]  = { 1.0f, 1.0f, 0.0f, 1.0f };

static GLfloat mygray10Percent[] = { 0.1f, 0.1f, 0.1f, 1.0f };
static GLfloat mygray20Percent[] = { 0.2f, 0.2f, 0.2f, 1.0f };
static GLfloat mygray30Percent[] = { 0.3f, 0.3f, 0.3f, 1.0f };

//***********************************
// Configuration struct for the application window
struct MyWindow
{
 string title;
 GLint posX;
 GLint posY;
 GLint width;
 GLint height;
};

// Controls the global world translation and rotation
struct MyWorld
{
 GLdouble rotateX, rotateY, rotateZ;
 GLdouble posX, posY, posZ;
 GLdouble scaleX, scaleY, scaleZ;

 // Translate the entire world
 void move(GLfloat xinc, GLfloat yinc, GLfloat zinc)
 {
  posX += xinc;
  posY += yinc;
  posZ += zinc;
 }

 // Rotate the entire world
 void rotate(GLfloat xinc, GLfloat yinc, GLfloat zinc)
 {
  rotateX += xinc;
  rotateY += yinc;
  rotateZ += zinc;
 }
};

// Configuration for the camera/viewer (gluLookAt and gluPerspective)
struct MyViewer
{
 GLdouble eyeX, eyeY, eyeZ;         // Camera position
 GLdouble centerX, centerY, centerZ;// Look-at target point
 GLdouble upX, upY, upZ;            // Up vector orientation

 GLdouble zNear, zFar;   // The distance to the near/far clipping plane
 GLdouble fieldOfView;   // Angle at Y direction (FOV)
 GLdouble aspectRatio;   // width / height of the viewport
};

// Application settings for interaction
struct MySetting
{
 GLdouble posInc;        // Translation increment speed
 GLdouble angleInc;      // Rotation increment speed
 int mouseX, mouseY;     // Previous mouse coordinates
 bool mouseRightMode;    // Is right mouse button held down
 bool mouseLeftMode;     // Is left mouse button held down

 bool shadingMode;       // Toggle for Solid/Wireframe mode
};

// Class for rendering XYZ Coordinate Axes in the scene
class MyAxis
{
 private:
    GLfloat xlen, ylen, zlen;
    GLfloat xred, xgreen, xblue;
    GLfloat yred, ygreen, yblue;
    GLfloat zred, zgreen, zblue;
    GLint    linestipplerepeatfactor;
    GLfloat  linewidth;
    GLushort linestipplepattern;
    bool     shoulddraw;
 public:
    MyAxis()
    {
     setXColor(1.0f, 0.0f, 0.0f); // Red X-axis
     setYColor(0.0f, 1.0f, 0.0f); // Green Y-axis
     setZColor(0.0f, 0.0f, 1.0f); // Blue Z-axis
     setLength(20.0f, 20.0f, 20.0f);
     setLineStipple(1, 0xffff);
     setLineWidth(1);
     shoulddraw = true;
    }

    void setLength(GLfloat xLen, GLfloat yLen, GLfloat zLen)
       { xlen = xLen; ylen = yLen; zlen = zLen; }
    void setXColor(GLfloat r, GLfloat g, GLfloat b)
       { xred = r; xgreen = g; xblue = b; }
    void setYColor(GLfloat r, GLfloat g, GLfloat b)
       { yred = r; ygreen = g; yblue = b; }
    void setZColor(GLfloat r, GLfloat g, GLfloat b)
       { zred = r; zgreen = g; zblue = b; }
    void setLineWidth(GLfloat lineWidth)
       { linewidth = lineWidth; }
    void setLineStipple(GLint repeatfactor, GLushort pattern)
       { linestipplerepeatfactor = repeatfactor, linestipplepattern = pattern; }

    void draw()
    {
     if (!shoulddraw) return;

     // Temporarily disable lighting to draw raw colored lines
     GLboolean lightingIsOn;
     glGetBooleanv(GL_LIGHTING, &lightingIsOn);
     glDisable(GL_LIGHTING);

     // Store current stipple states to restore them later
     GLint oldlinestipplerepeatfactor;
     GLint oldlinestipplepattern;
     GLfloat  oldlinewidth;
     GLboolean stippleIsOn;
     glGetIntegerv(GL_LINE_STIPPLE_REPEAT, &oldlinestipplerepeatfactor);
     glGetIntegerv(GL_LINE_STIPPLE_PATTERN, &oldlinestipplepattern);
     glGetFloatv(GL_LINE_WIDTH, &oldlinewidth);
     glGetBooleanv(GL_LINE_STIPPLE,&stippleIsOn);

     glEnable(GL_LINE_STIPPLE);
     glLineWidth(linewidth);
     glLineStipple(linestipplerepeatfactor, linestipplepattern);

     glBegin(GL_LINES);
        // X-axis
        glColor3f(xred, xgreen, xblue);
        glVertex3f( 0.0, 0.0, 0.0);
        glVertex3f(xlen, 0.0, 0.0);
        // X-axis arrowhead
        glVertex3f(    xlen, 0.0,  0.0);
        glVertex3f(0.9*xlen, 0.0,  0.05*xlen);
        glVertex3f(    xlen, 0.0,  0.0);
        glVertex3f(0.9*xlen, 0.0, -0.05*xlen);

        // Y-axis
        glColor3f(yred, ygreen, yblue);
        glVertex3f(0.0,  0.0, 0.0);
        glVertex3f(0.0, ylen, 0.0);
        // Y-axis arrowhead
        glVertex3f(       0.0,     ylen, 0.0);
        glVertex3f( 0.05*ylen, 0.9*ylen, 0.0);
        glVertex3f(       0.0,     ylen, 0.0);
        glVertex3f(-0.05*ylen, 0.9*ylen, 0.0);

        // Z-axis
        glColor3f(zred, zgreen, zblue);
        glVertex3f(0.0, 0.0,  0.0);
        glVertex3f(0.0, 0.0, zlen);
        // Z-axis arrowhead
        glVertex3f(0.0, 0.0, zlen);
        glVertex3f( 0.05*zlen, 0.0, 0.9*zlen);
        glVertex3f(       0.0, 0.0,     zlen);
        glVertex3f(-0.05*zlen, 0.0, 0.9*zlen);
     glEnd();

     // Restore previous OpenGL states
     glLineWidth(oldlinewidth);
     glLineStipple(oldlinestipplerepeatfactor, oldlinestipplepattern);
     if (stippleIsOn==GL_FALSE) glDisable(GL_LINE_STIPPLE);
     if (lightingIsOn==GL_TRUE) glEnable(GL_LIGHTING);
    }

    void toggle() { shoulddraw = !shoulddraw; }
    void enable() { shoulddraw = true; }
    void disable() { shoulddraw = false; }
};

#endif //YP_CGLABMAIN_HPP

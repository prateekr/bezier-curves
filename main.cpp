#include <iostream>
#include <Eigen/Dense>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#ifdef OSX
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif

#include "base.h"
#include "window.h"
#include "parser.h"
#include "scene.h"

inline float sqr(float x) { return x*x; }

using namespace Eigen;

Window window = Window(400, 400);
Scene *scene = new Scene();


void myReshape(int w, int h) {
  window.width = w;
  window.height = h;

  glViewport (0,0,3,3);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(-1.5, 1.5, -1.5, 1.5);
}

//****************************************************
// Draw a filled circle.  
//****************************************************
void circle() {
  // Draw inner circle

  for (int i = 0; i < 1; i++) {
    //glBegin(GL_LINES);
   // window.drawPoints(scene->curves.at(i).getPoints(0.001));
   // glEnd();
  }

  glBegin(GL_POINTS);
  //window.drawLine(Line(Point(0,0,0), Point(1, 1, 0)));
    
    //CubicBezier curve = scene->curves.at(0);
    //window.drawCurve(curve, 0.001f);
  glEnd();
  
  //glBegin(GL_LINE_LOOP); // Start drawing a line primitive  
  //glVertex3f(-1.0f, -1.0f, 0.0f); // The bottom left corner  
  //glVertex3f(-1.0f, 1.0f, 0.0f); // The top left corner  
  //glVertex3f(1.0f, 1.0f, 0.0f); // The top right corner  
  //glVertex3f(1.0f, -1.0f, 0.0f); // The bottom right corner  
  //glEnd();  


  //window.drawPoints(curve.getPoints(0.001));
  //window.drawLine(Line(Point(0,0,0), Point(1, 0, 0)));

}

//****************************************************
// function that does the actual drawing of stuff
//***************************************************
void myDisplay() {
  glClear(GL_COLOR_BUFFER_BIT);	// clear the color buffer

  glMatrixMode(GL_MODELVIEW);	// indicate we are specifying camera transformations
  glLoadIdentity();				    // make sure transformation is "zero'd"

  // Start drawing
  circle();

  glFlush();
  glutSwapBuffers();	// swap buffers (we earlier set double buffer)
}



//****************************************************
// the usual stuff, nothing exciting here
//****************************************************
int asdf(int argc, char *argv[]) {
  Parser parser = Parser();
  parser.parseFile("input files/input1.bez", scene);
  
  //This initializes glut
  glutInit(&argc, argv);

  //This tells glut to use a double-buffered window with red, green, and blue channels 
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

  //The size and position of the window
  glutInitWindowSize(window.width, window.height);
  glutInitWindowPosition(0,0);
  glutCreateWindow(argv[0]);

  glutDisplayFunc(myDisplay);	// function to run when its time to draw something
  glutReshapeFunc(myReshape);	// function to run when the window gets resized  
  
  glutMainLoop();	// infinite loop that will keep drawing and resizing and whatever else

  return 0;
}

// Clears the current window and draws a triangle.
void display() {

  // Set every pixel in the frame buffer to the current clear color.
  glClear(GL_COLOR_BUFFER_BIT);

  // Drawing is done by specifying a sequence of vertices.  The way these
  // vertices are connected (or not connected) depends on the argument to
  // glBegin.  GL_POLYGON constructs a filled polygon.
  glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

  for (int i = 0; i < 1; i+=4) {
    glBegin(GL_POLYGON);
     glColor3f(1, 0, 0);
      for (int j = i; j < i + 4; j++) {
        CubicBezier curve = scene->curves.at(j);
        window.drawCurvePolygonMode(curve, 0.001f);
      }
    glEnd();
  }

  //window.drawLine(Line(Point(0,0,0), Point(1, 1, 0)));
  //CubicBezier curve = scene->curves.at(0);
  //window.drawCurve(curve, 0.001f);

  // Flush drawing command buffer to make drawing happen as soon as possible.
  glFlush();
  glutSwapBuffers();
}

// Initializes GLUT, the display mode, and main window; registers callbacks;
// enters the main event loop.
int main(int argc, char** argv) {
  Parser parser = Parser();
  parser.parseFile("input files/input1.bez", scene);
  // Use a single buffered window in RGB mode (as opposed to a double-buffered
  // window or color-index mode).
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

  // Position window at (80,80)-(480,380) and give it a title.
  glutInitWindowPosition(80, 80);
  glutInitWindowSize(400, 300);
  glutCreateWindow("A Simple Triangle");

  // Tell GLUT that whenever the main window needs to be repainted that it
  // should call the function display().
  glutDisplayFunc(display);

  // Tell GLUT to start reading and processing events.  This function
  // never returns; the program only exits when the user closes the main
  // window or kills the process.
  glutMainLoop();
}
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
Scene scene = Scene();

// Clears the current window and draws a triangle.
void display() {
  //glClear(GL_COLOR_BUFFER_BIT);
   glClear (GL_COLOR_BUFFER_BIT);
   glColor3f (1.0, 1.0, 1.0);
   glOrtho(-5, 5, -5, 5, -5.0, 5.0);
   //glOrtho(-2.1, 2.1, -2.1, 2.1, -5.0, 5.0); //define left, right, bottom, top, nearVal, farVal
  // Drawing is done by specifying a sequence of vertices.  The way these
  // vertices are connected (or not connected) depends on the argument to
  // glBegin.  GL_POLYGON constructs a filled polygon.
  glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
  /*
    glBegin(GL_POINTS);
    glColor3f(1, 0, 0);
    for (float u = 0; u <= 1; u+=0.33) {
      for (float v = 0; v <= 1; v+=0.33) {
        //Point p = curve->at(u, v);
        //glVertex3f(p.x(), p.y(), p.z());
      }
    }
    glEnd();
    */
  
  glRotatef(-120, 1.0, 0, 0);

  for (int i = 0; i < scene.patches.size(); i++) {
    window.drawWireMesh(*scene.patches.at(i),1.0f/10.0f);
  }
  
  //glPopMatrix();
  glFlush();
  glutSwapBuffers();
}

// Initializes GLUT, the display mode, and main window; registers callbacks;
// enters the main event loop.
int main(int argc, char** argv) {
  Parser parser = Parser();
  parser.parseFile("input files/input3.bez", &scene);
  // Use a single buffered window in RGB mode (as opposed to a double-buffered
  // window or color-index mode).
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

  // Position window at (80,80)-(480,380) and give it a title.
  glutInitWindowPosition(80, 80);
  glutInitWindowSize(800, 800);
  glutCreateWindow("A Simple Triangle");

  // Tell GLUT that whenever the main window needs to be repainted that it
  // should call the function display().
  glutDisplayFunc(display);

  // Tell GLUT to start reading and processing events.  This function
  // never returns; the program only exits when the user closes the main
  // window or kills the process.
  glutMainLoop();
}
#include <iostream>
#include "Eigen/Dense"

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
int window_id;


void init()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// background color
  //glClearDepth(1.0);			// background depth value

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-5, 5, -5, 5, -5.0, 5.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //Comment out if you want fill.


  float lpos[] = { 0.0f, 0.0f, -10.0f, 0 };
  glLightfv(GL_LIGHT0, GL_POSITION, lpos);
  
  glEnable(GL_DEPTH_TEST);		// enable hidden surface removal
  glEnable(GL_LIGHTING);		// enable lighting
  glEnable(GL_LIGHT0);		// enable
	
  // glShadeModel(GL_FLAT);		// flat shading
  glShadeModel(GL_SMOOTH);		// smooth shading 

  glRotatef(270.0f, 1, 0, 0);
  //glRotatef(180.0f, 1, 0, 0);

}


void display() {
  for (int i = 0; i < scene.patches.size(); i++) {
    //window.drawWireMesh(*scene.patches.at(i),1.0f/50.0f);
    window.drawSurfacePointMode(*scene.patches.at(i),1.0f/20.0f);
  }

  // glFlush();
  glutSwapBuffers();
}

//****************************************************
// Key Pressing
//****************************************************

void keyPressed (unsigned char key, int x, int y) {
  float rotation_degree = 10.0f;
  if (key == 43) { // + is pressed
    std::cout << "asdf" << std::endl;
    glTranslatef(0, 0, 2);
  }
  if (key == 45) { // - is pressed

  }
  if (key == 32) { // If the space key is pressed
    glutDestroyWindow ( window_id );
    exit(0);
  }
}

void keyPressed2 (int key, int x, int y) {
  float rotation_degree = 10.0f;
  glMatrixMode(GL_PROJECTION);

  if (key == GLUT_KEY_UP) {
    if (glutGetModifiers() & GLUT_ACTIVE_SHIFT) {
      glTranslatef(0,0.5,0);
    }
    else {
      glRotatef(rotation_degree, -1, 0, 0);
    }
  }
  if (key == GLUT_KEY_DOWN) {
    if (glutGetModifiers() & GLUT_ACTIVE_SHIFT) {
      glTranslatef(0,-0.5,0);
    }
    else {
      glRotatef(rotation_degree, 1, 0, 0);
    }
  }
  if (key == GLUT_KEY_RIGHT) {
    if (glutGetModifiers() & GLUT_ACTIVE_SHIFT) {
      glTranslatef(0.5,0,0);
    }
    else {
      glRotatef(rotation_degree, 0, 0, 1);
    }
  }
  if (key == GLUT_KEY_LEFT) {
    if (glutGetModifiers() & GLUT_ACTIVE_SHIFT) {
      glTranslatef(-0.5,0,0);
    }
    else {
      glRotatef(rotation_degree, 0, 0, -1);
    }
  }
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glutPostRedisplay();
}

// Initializes GLUT, the display mode, and main window; registers callbacks;
// enters the main event loop.
int main(int argc, char** argv) {
  Parser parser = Parser();
  parser.parseFile("input_files/input3.bez", &scene);
  // Use a single buffered window in RGB mode (as opposed to a double-buffered
  // window or color-index mode).
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

  // glMatrixMode(GL_MODELVIEW);

  // Position window at (80,80)-(480,380) and give it a title.
  glutInitWindowPosition(80, 80);
  glutInitWindowSize(800, 800);
  window_id = glutCreateWindow("A Simple Triangle");
  init();

  // Tell GLUT that whenever the main window needs to be repainted that it
  // should call the function display().
  glutDisplayFunc(display);

  // Handle Key Pressing
  glutKeyboardFunc(keyPressed); // Tell GLUT to use the method "keyPressed" for key presses
  glutSpecialFunc(keyPressed2);

  // Tell GLUT to start reading and processing events.  This function
  // never returns; the program only exits when the user closes the main
  // window or kills the process.
  glutMainLoop();
}

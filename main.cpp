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
GLuint obj_id;
GLdouble dist = 5;
GLdouble fov = 45;
bool shading_toggle = 0;
bool fill_toggle = 0;

void init()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// background color
  //glClearDepth(1.0);			// background depth value

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  // gluLookAt(0,0,3.5,0,0,0,0,1,0);
  // gluPerspective(fov, 1, -100, 100);
  glOrtho(-dist, dist, -dist, dist, -100, 100);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  // glFrustum(-5,5,-5,5,-5,5);
  // gluLookAt(0,0,3.5,0,0,0,0,1,0);

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //Comment out if you want fill.


  GLfloat lpos[] = { 0.0f, 0.0f, -10.0f, 0 };
  glLightfv(GL_LIGHT0, GL_POSITION, lpos);
  GLfloat ldiff[] = { 1.0f, 0.3f, 0.6f, 1.0 };
  glLightfv(GL_LIGHT0, GL_DIFFUSE, ldiff);
  GLfloat lspec[] = { 1.0f, 1.0f, 1.0f, 1.00 };
  glLightfv(GL_LIGHT0, GL_SPECULAR, lspec);
  
  GLfloat lpos1[] = { 0.0f, -10.0f, 0.0f, 0 };
  glLightfv(GL_LIGHT1, GL_POSITION, lpos);
  GLfloat ldiff1[] = { 1.0f, 0.3f, 0.6f, 1.0 };
  glLightfv(GL_LIGHT1, GL_DIFFUSE, ldiff);
  GLfloat lspec1[] = { 0.5f, 0.5f, 0.5f, 1.00 };
  glLightfv(GL_LIGHT1, GL_SPECULAR, lspec);

  glEnable(GL_DEPTH_TEST);		// enable hidden surface removal
  glEnable(GL_LIGHTING);		// enable lighting
  glEnable(GL_LIGHT0);		// enable
  glEnable(GL_LIGHT1);
 
  // glRotatef(270.0f, 1, 0, 0);
  //glRotatef(180.0f, 1, 0, 0);
}

void createObj() {
  obj_id = glGenLists(1);
  glNewList(obj_id, GL_COMPILE);
  for (int i = 0; i < scene.patches.size(); i++) {
    // window.drawAdaptive(*scene.patches.at(i),0.1f);
    window.drawUniform(*scene.patches.at(i),1.0f/10.0f);
  }
  glEndList();
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   // background color

  glCallList(obj_id);

  // glFlush();
  glutSwapBuffers();
}

//****************************************************
// Key Pressing
//****************************************************

void keyPressed (unsigned char key, int x, int y) {
  float rotation_degree = 10.0f;
  if ((key == 43) || (key == 61)) { // + is pressed
    dist -= 0.5;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-dist, dist, -dist, dist, -100, 100);
    glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay();
  }
  else if ((key == 45) || (key == 95)) { // - is pressed
    dist += 0.5;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-dist, dist, -dist, dist, -100, 100);
    glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay();
  }
  else if (key == 115) { // s is pressed
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    fill_toggle = 0;
    if (shading_toggle) {
      glShadeModel(GL_SMOOTH);
    }
    else {
      glShadeModel(GL_FLAT);
    }

    shading_toggle = !shading_toggle;
    glutPostRedisplay();
  }
  else if (key == 119) { // w is pressed
    if (fill_toggle) {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    else {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    fill_toggle = !fill_toggle;
    glutPostRedisplay();
  }
  else if (key == 32) { // If the space key is pressed
    glutDestroyWindow ( window_id );
    exit(0);
  }
}

void keyPressed2 (int key, int x, int y) {
  float rotation_degree = 10.0f;

  if (key == GLUT_KEY_UP) {
    if (glutGetModifiers() & GLUT_ACTIVE_SHIFT) {
      glTranslatef(0,0.5,0);
    }
    else {
      glRotatef(rotation_degree, -1, 0, 0);
    }
  }
  else if (key == GLUT_KEY_DOWN) {
    if (glutGetModifiers() & GLUT_ACTIVE_SHIFT) {
      glTranslatef(0,-0.5,0);
    }
    else {
      glRotatef(rotation_degree, 1, 0, 0);
    }
  }
  else if (key == GLUT_KEY_RIGHT) {
    if (glutGetModifiers() & GLUT_ACTIVE_SHIFT) {
      glTranslatef(0.5,0,0);
    }
    else {
      glRotatef(rotation_degree, 0, 0, 1);
    }
  }
  else if (key == GLUT_KEY_LEFT) {
    if (glutGetModifiers() & GLUT_ACTIVE_SHIFT) {
      glTranslatef(-0.5,0,0);
    }
    else {
      glRotatef(rotation_degree, 0, 0, -1);
    }
  }
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

  // Position window at (80,80)-(480,380) and give it a title.
  glutInitWindowPosition(80, 80);
  glutInitWindowSize(800, 800);
  window_id = glutCreateWindow("Teapot");
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  init();
  createObj();

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

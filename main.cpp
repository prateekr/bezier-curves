#include <iostream>

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

#include "Eigen/Dense"
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

  glViewport (0,0,window.width,window.height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, window.width, 0, window.height);
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

  //glBegin(GL_LINE);
  //window.drawLine(Line(Point(0,0,0), Point(100, 100, 0)), 0.001);
    
    //CubicBezier curve = scene->curves.at(0);
    //window.drawPoints(curve.getPoints(0.001));
  //glEnd();
  
  glBegin(GL_LINE_LOOP); // Start drawing a line primitive  
  glVertex3f(-1.0f, -1.0f, 0.0f); // The bottom left corner  
  glVertex3f(-1.0f, 1.0f, 0.0f); // The top left corner  
  glVertex3f(1.0f, 1.0f, 0.0f); // The top right corner  
  glVertex3f(1.0f, -1.0f, 0.0f); // The bottom right corner  
  glEnd();  


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

void Draw() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINES);
		glVertex3f(0.25, 0.25, 0.0);
		glVertex3f(0.75, 0.75, 0.0);
	glEnd();
	glFlush();
  glutSwapBuffers();	// swap buffers (we earlier set double buffer)
}

void Initialize() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
  gluOrtho2D(-3, 3, -3, 3);
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(250, 250);
	glutInitWindowPosition(200, 200);
	glutCreateWindow("XoaX.net");
	Initialize();
	glutDisplayFunc(Draw);
	glutMainLoop();
	return 0;
}

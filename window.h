#ifndef WINDOW_H
#define WINDOW_H

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

using namespace Eigen;

class Window {
  public:
    int height, width;

    Window(int height, int width);
    void setPixel(int i, int j,  GLfloat r, GLfloat g, GLfloat b);
    void drawLine(Line line, float precision);
    void drawPoints(std::vector<Point> points);
};


#endif
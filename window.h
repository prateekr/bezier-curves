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
    void drawPoint(Point p);
    void drawLine(Line line);
    void drawTriangle(Point p1, Point p2, Point p3);
    void drawCurvePointMode(CubicBezier curve, float precision);
    void drawCurveLineMode(CubicBezier curve, float precision);
    void drawCurvePolygonMode(CubicBezier curve, float precision);
    void drawWireMesh(BezierPatch patch, float precision);
    void drawSurfacePointMode(BezierPatch patch, float precision);
    void adaptiveTessellate(BezierPatch patch, float precision, Point uv1, Point uv2, Point uv3);
    int checkMidpoint(BezierPatch patch, float precision, Point p1, Point p2);
    void shiftLeft(Point &uv1, Point &uv2, Point &uv3);
};

#endif
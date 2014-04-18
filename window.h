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
#include "output.h"

using namespace Eigen;

class Window {
  public:
    int height, width;
    float epsilon; // small value to add to vectors so they aren't 0
    Output output;
    bool out; // flag for if we are exporting

    Window() {};
    Window(int height, int width, Output o, bool o2);
    void setPixel(int i, int j,  GLfloat r, GLfloat g, GLfloat b);
    void drawPoint(Point p);
    void drawLine(Line line);
    void drawTriangle(Vertex v1, Vertex v2, Vertex v3);
    void drawQuad(Vertex ul, Vertex ur, Vertex lr, Vertex ll);
    void drawCurvePointMode(CubicBezier curve, float precision);
    void drawCurveLineMode(CubicBezier curve, float precision);
    void drawCurvePolygonMode(CubicBezier curve, float precision);
    void drawAdaptive(BezierPatch patch, float precision);
    void drawUniform(BezierPatch patch, float precision);
    void adaptiveTessellate(BezierPatch patch, float precision, Point uv1, Point uv2, Point uv3);
    int checkMidpoint(BezierPatch patch, float precision, Point p1, Point p2);
    void shiftLeft(Point &uv1, Point &uv2, Point &uv3);
};

#endif
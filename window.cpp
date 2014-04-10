#include "window.h"


Window::Window(int w,int h) {
  width = w;
  height = h;
}

void Window::setPixel(int x, int y, GLfloat r, GLfloat g, GLfloat b) {
  glColor3f(r, g, b);
  glVertex2f(x + 0.5, y + 0.5);
}

void Window::setPoint(float x, float y, float z) {
  glVertex3f(x, y, z);
}

void Window::drawLine(Line line) {
  glVertex3f(line.origin.x(), line.origin.y(), line.origin.z());
  glVertex3f(line.destination.x(), line.destination.y(), line.destination.z());
}

void Window::drawCurvePointMode(CubicBezier curve, float precision) {
  glBegin(GL_POINTS);
  for (float u = 0; u <= 1; u += precision) {
    Point p = curve.at(u);
    glVertex3f(p.x(), p.y(), p.z());
  }
  glEnd();
}

void Window::drawCurveLineMode(CubicBezier curve, float precision) {
  glBegin(GL_LINES);
  for (float u = precision; u <= 1; u += precision) {
    drawLine(Line(curve.at(u-precision),curve.at(u)));
  }
  glEnd();
}

void Window::drawWireMesh(BezierPatch patch, float precision) {
  glBegin(GL_LINES);
  for (float u = 0; u <= 1+precision; u+= precision) {
    for (float v = precision; v <= 1+precision; v+= precision) {
      Point o = patch.at2(u,v-precision), d = patch.at2(u,v);
      drawLine(Line(o,d));
    }
  }
  
  for (float v = 0; v <= 1+precision; v += precision) {
    for (float u = precision; u <= 1+precision; u+= precision) {
      drawLine(Line(patch.at2(u-precision,v),patch.at2(u,v)));
    }
  }
  glEnd();


}

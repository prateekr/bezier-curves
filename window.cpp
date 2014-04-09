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

void Window::drawCurveLineMode(BezierCurve curve, float precision) {
  std::vector<Point> points;
  curve.getPoints(precision, &points);

  for (int i = 1; i < points.size(); i++) {
    drawLine(Line(points.at(i-1), points.at(i)));
  }
}

void Window::drawCurvePolygonMode(BezierCurve curve, float precision) {
  std::vector<Point> points;
  curve.getPoints(precision, &points);

  for (int i = 0; i < points.size(); i++) {
      glVertex3f(points.at(i).x(), points.at(i).y(), points.at(i).z());
  }
}

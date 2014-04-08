#include "window.h"


Window::Window(int w,int h) {
  width = w;
  height = h;
}

void Window::setPixel(int x, int y, GLfloat r, GLfloat g, GLfloat b) {
  glColor3f(r, g, b);
  glVertex2f(x + 0.5, y + 0.5);
}

void Window::drawLine(Line line, float precision) {
  for (float u = 0; u <= 1; u+=precision) {
    Point point = line.getPoint(u);
    setPixel(point.x(),point.y(), 1.0, 0.0, 0.0);
  }
}

void Window::drawPoints(std::vector<Point> points) {
  for (int i = 0; i < points.size(); i++) {
    setPixel(points.at(i).x(), points.at(i).y(), 1.0, 0.0, 0.0);
  }
}
#include "window.h"

Window::Window(int w,int h) {
  width = w;
  height = h;
}

void Window::setPixel(int x, int y, GLfloat r, GLfloat g, GLfloat b) {
  glColor3f(r, g, b);
  glVertex2f(x + 0.5, y + 0.5);
}

void Window::drawPoint(Point p) {
  glVertex3f(p.x(), p.y(), p.z());
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
  for (float u = 0; u <= 1+precision/2; u+= precision) {
    for (float v = precision; v <= 1+precision/2; v+= precision) {
      Point o = patch.at(u,v-precision), d = patch.at(u,v);
      drawLine(Line(o,d));
    }
  }
  
  for (float v = 0; v <= 1+precision/2; v += precision) {
    for (float u = precision; u <= 1+precision/2; u+= precision) {
      drawLine(Line(patch.at(u-precision,v),patch.at(u,v)));
    }
  }

  glEnd();
}

void Window::drawSurfacePointMode(BezierPatch patch, float precision) {
  std::vector< std::vector<Point> > *grid = new std::vector< std::vector<Point> >();
  patch.getGridPoints(grid, precision);
  for (int i = 0; i < grid->size()-1; i++) { //rows
    for (int j = 0; j < grid->at(i).size()-1; j++) { //columns
      Point ul = grid->at(i+1).at(j), ur = grid->at(i+1).at(j+1);
      Point ll = grid->at(i).at(j), lr = grid->at(i).at(j+1);

      glBegin(GL_POLYGON);
      drawPoint(ul);
      drawPoint(ur);
      drawPoint(lr);
      drawPoint(ll);
      glEnd();
    }
  }
}

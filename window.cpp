#include "window.h"
#include <iostream>
#include <stdlib.h>

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

void Window::drawTriangle(Point p1, Point p2, Point p3) {
  glBegin(GL_TRIANGLES);
  glVertex3f(p1.x(), p1.y(), p1.z());
  glVertex3f(p2.x(), p2.y(), p2.z());
  glVertex3f(p3.x(), p3.y(), p3.z());
  glEnd();
}

void Window::drawQuad(Point ul, Point ur, Point lr, Point ll) {
  Vector3f normal = (ul - ll).cross(lr - ll);
  if (normal != Point(0,0,0)) {
    normal.normalize();
  } /*else {
    Vector3f zero_vector(0.0f, 0.0f, 0.0f);
    Vector3f v1 = ul - ll;
    Vector3f v2 = lr - ll;
    if (v1 == zero_vector) {
      v1 = Point(0.00001, 0.00001, 0.00001);
    }
    if (v2 == zero_vector) {
      v2 = Point(-0.00001, -0.00001, -0.00001);
    }

    normal = v1.cross(v2);
    //normal = Vector3f(1,1,1);
    if (normal != zero_vector) {
      normal.normalize();
    } else {
      normal = Vector3f(1, 0, 0);
      normal.normalize();
    }
  }*/
  glBegin(GL_QUADS);
  glNormal3f(normal.x(), normal.y(), normal.z());
  glVertex3f(ul.x(), ul.y(), ul.z());
  glVertex3f(ur.x(), ur.y(), ur.z());
  glVertex3f(lr.x(), lr.y(), lr.z());
  glVertex3f(ll.x(), ll.y(), ll.z());
  glEnd();
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
  adaptiveTessellate(patch, precision, Point(0,0,0), Point(1,1,0), Point(1,0,0));
  adaptiveTessellate(patch, precision, Point(1,1,0), Point(0,0,0), Point(0,1,0));
}

// Pass in the (u,v) coord of the points of the triangle.
// Perform test and then, recursively call
// The arguments uv1-uv3 are (u, v, 0). They store the screen coord, NOT REAL WORLD COORD
void Window::adaptiveTessellate(BezierPatch patch, float precision, Point uv1, Point uv2, Point uv3) {
  assert((uv1.z() + uv2.z() + uv3.z()) == 0);
  std::vector<int> *midPoints = new std::vector<int>();
  Point m;

  midPoints->push_back(checkMidpoint(patch, precision, uv1, uv2)); // m3
  midPoints->push_back(checkMidpoint(patch, precision, uv1, uv3)); // m2
  midPoints->push_back(checkMidpoint(patch, precision, uv2, uv3)); // m1

  int cracks = 0;
  for (int i = 0; i < midPoints->size(); i++) {
    // std::cout << midPoints->at(i) << std::endl;
    cracks += midPoints->at(i);
  }

  if (cracks == 0) {
    drawTriangle(patch.at(uv1.x(), uv1.y()), patch.at(uv2.x(), uv2.y()), patch.at(uv3.x(), uv3.y()));
  }
  else if (cracks == 1) {
    if (midPoints->at(1) == 1) {
      shiftLeft(uv1, uv2, uv3);
      shiftLeft(uv1, uv2, uv3);
    }
    else if (midPoints->at(2) == 1) {
      shiftLeft(uv1, uv2, uv3); 
    }

    //recurse on 1 split
    adaptiveTessellate(patch, precision, uv1, (uv1 + uv2) / 2, uv3);
    adaptiveTessellate(patch, precision, (uv1 + uv2) / 2, uv2, uv3);
  }
  else if (cracks == 2) {
    if (midPoints->at(0) == 0) { //m3
      shiftLeft(uv1, uv2, uv3);
    }
    else if (midPoints->at(2) == 0) { //m1
      shiftLeft(uv1, uv2, uv3);
      shiftLeft(uv1, uv2, uv3);
    }

    adaptiveTessellate(patch, precision, uv1, (uv1 + uv2) / 2, uv3);
    adaptiveTessellate(patch, precision, (uv1 + uv2) / 2, (uv2 + uv3) / 2, uv3);
    adaptiveTessellate(patch, precision, (uv1 + uv2) / 2, uv2, (uv2 + uv3) / 2);
  }
  else if (cracks == 3) {
    adaptiveTessellate(patch, precision, uv1, (uv1 + uv2) / 2, (uv1 + uv3) / 2);
    adaptiveTessellate(patch, precision, (uv1 + uv2) / 2, uv2, (uv2 + uv3) / 2);
    adaptiveTessellate(patch, precision, (uv1 + uv3) / 2, (uv2 + uv3) / 2, uv3);
    adaptiveTessellate(patch, precision, (uv1 + uv2) / 2, (uv2 + uv3) / 2, (uv1 + uv3) / 2);
  }
}

// Return 1 if midpoint value is NOT within the acceptable precision value and 0 if it is
int Window::checkMidpoint(BezierPatch patch, float precision, Point uv1, Point uv2) {
  assert((uv1.z() + uv2.z()) == 0);
  Point m = (uv1 + uv2) / 2;
  Point p1 = patch.at(uv1.x(), uv1.y());
  Point p2 = patch.at(uv2.x(), uv2.y());
  Point midP = patch.at(m.x(), m.y());
  Point val = midP - ((p1 + p2) / 2);
  float mag = sqrt(pow(val.x(),2) + pow(val.y(),2) + pow(val.z(),2));
  return (mag >= precision);
}

void Window::shiftLeft(Point &uv1, Point &uv2, Point &uv3) {
  Point temp = uv1;
  uv1 = uv2;
  uv2 = uv3;
  uv3 = temp;
}

void Window::drawSurfacePointMode(BezierPatch patch, float precision) {
  std::vector< std::vector<Point> > *grid = new std::vector< std::vector<Point> >();
  patch.getGridPoints(grid, precision);
  for (int i = 0; i < grid->size()-1; i++) { //rows
    for (int j = 0; j < grid->at(i).size()-1; j++) { //columns
      Point ul = grid->at(i+1).at(j), ur = grid->at(i+1).at(j+1);
      Point ll = grid->at(i).at(j), lr = grid->at(i).at(j+1);
      drawQuad(ul, ur, lr, ll);
    }
  }
}

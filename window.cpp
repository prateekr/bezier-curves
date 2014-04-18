#include "window.h"
#include <iostream>
#include <stdlib.h>

Window::Window(int w,int h) {
  width = w;
  height = h;
  epsilon = 0.01;
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

void Window::drawTriangle(Vertex v1, Vertex v2, Vertex v3) {
  // Vector3f normal = (p1 - p2).cross(p3 - p2);
  // if (normal != Point(0,0,0)) {
  //   normal.normalize();
  // }

  glBegin(GL_TRIANGLES);
  glNormal3f(v1.normal.x(), v1.normal.y(), v1.normal.z());
  glVertex3f(v1.point.x(), v1.point.y(), v1.point.z());

  glNormal3f(v2.normal.x(), v2.normal.y(), v2.normal.z());
  glVertex3f(v2.point.x(), v2.point.y(), v2.point.z());

  glNormal3f(v3.normal.x(), v3.normal.y(), v3.normal.z());
  glVertex3f(v3.point.x(), v3.point.y(), v3.point.z());
  glEnd();
}

void Window::drawQuad(Vertex ul, Vertex ur, Vertex lr, Vertex ll) {
  glBegin(GL_QUADS);
  glNormal3f(ul.normal.x(), ul.normal.y(), ul.normal.z());
  glVertex3f(ul.point.x(), ul.point.y(), ul.point.z());
  
  glNormal3f(ur.normal.x(), ur.normal.y(), ur.normal.z());
  glVertex3f(ur.point.x(), ur.point.y(), ur.point.z());
  
  glNormal3f(lr.normal.x(), lr.normal.y(), lr.normal.z());
  glVertex3f(lr.point.x(), lr.point.y(), lr.point.z());
  
  glNormal3f(ll.normal.x(), ll.normal.y(), ll.normal.z());
  glVertex3f(ll.point.x(), ll.point.y(), ll.point.z()); 
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

void Window::drawAdaptive(BezierPatch patch, float precision) {
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
    drawTriangle(patch.vertex_at(uv1.x(), uv1.y()), patch.vertex_at(uv2.x(), uv2.y()), patch.vertex_at(uv3.x(), uv3.y()));
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
  Vertex v1 = patch.vertex_at(uv1.x(), uv1.y());
  Vertex v2 = patch.vertex_at(uv2.x(), uv2.y());
  Vertex midV = patch.vertex_at(m.x(), m.y());
  Point val = midV.point - ((v1.point + v2.point) / 2);
  float mag = sqrt(pow(val.x(),2) + pow(val.y(),2) + pow(val.z(),2));
  return (mag >= precision);
}

void Window::shiftLeft(Point &uv1, Point &uv2, Point &uv3) {
  Point temp = uv1;
  uv1 = uv2;
  uv2 = uv3;
  uv3 = temp;
}

void Window::drawUniform(BezierPatch patch, float precision) {
  std::vector< std::vector<Vertex> > *grid = new std::vector< std::vector<Vertex> >();
  patch.getGridPoints(grid, precision);
  for (int i = 0; i < grid->size()-1; i++) { //rows
    for (int j = 0; j < grid->at(i).size()-1; j++) { //columns
      Vertex ul = grid->at(i+1).at(j), ur = grid->at(i+1).at(j+1);
      Vertex ll = grid->at(i).at(j), lr = grid->at(i).at(j+1);
      drawQuad(ul, ur, lr, ll);
    }
  }
}

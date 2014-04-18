#ifndef BASE_H
#define BASE_H

#include <math.h>

#include <vector>
#include <queue>
#include "Eigen/Dense"

using namespace Eigen;

typedef Vector3f Point;

class Line : public Vector3f {
  public:
    Point origin, destination;
    
    Line() { }

    Line(Point o, Point d) {
      origin = o, destination = d;
    }

    //Computes point on the line for u in range (0, 1)
    Point getPoint(float u) {
      return origin + u * (destination - origin);
    }
};

class Vertex {
  public:
    Point point;
    Vector3f normal;

    Vertex(Point p, Vector3f n) {
      point = p;
      normal = n;
    }
};

class CubicBezier {
  public:
    std::vector<Point> *points;

    CubicBezier() { }

    CubicBezier(std::vector<Point> *list) {
      points = list;
    }

    CubicBezier(Point p1, Point p2, Point p3, Point p4) {
      points = new std::vector<Point>();
      points->push_back(p1);
      points->push_back(p2);
      points->push_back(p3);
      points->push_back(p4);
    }

    Point at(float u) {
      Point p = pow(1-u,3) * points->at(0);
      p += 3*u*pow(1-u,2) * points->at(1);
      p += 3*pow(u,2)*(1-u) * points->at(2);
      p += pow(u,3) * points->at(3);
      return p;
    }

    Vector3f deriv_at(float u) {
      Point p0 = Line(points->at(0), points->at(1)).getPoint(u);
      Point p1 = Line(points->at(1), points->at(2)).getPoint(u);
      Point p2 = Line(points->at(2), points->at(3)).getPoint(u);

      Point p3 = Line(p0, p1).getPoint(u);
      Point p4 = Line(p1, p2).getPoint(u);

      Vector3f du = 3 * (p4 - p3);
      return du;
    }
};

class BezierPatch {
  public:
    std::vector< std::vector<Point> > *grid;
    std::vector<CubicBezier> *curves;
    std::vector<float> binomialCoeff;
    float n;

    BezierPatch(std::vector< std::vector<Point> > *g) {
      grid = g;
      curves = new std::vector<CubicBezier>();
      n = 3;

      for (int i = 0; i < g->size(); i++) {
        curves->push_back(CubicBezier(g->at(i).at(0),g->at(i).at(1),g->at(i).at(2),g->at(i).at(3)));
      }

      binomialCoeff.push_back(1);
      binomialCoeff.push_back(3);
      binomialCoeff.push_back(3);
      binomialCoeff.push_back(1);
    }

    void getGrid(std::vector<CubicBezier> *output) {
      output->push_back(CubicBezier(grid->at(0).at(0),grid->at(0).at(1),grid->at(0).at(2),grid->at(0).at(3)));
      output->push_back(CubicBezier(grid->at(1).at(0),grid->at(1).at(1),grid->at(1).at(2),grid->at(1).at(3)));
      output->push_back(CubicBezier(grid->at(2).at(0),grid->at(2).at(1),grid->at(2).at(2),grid->at(2).at(3)));
      output->push_back(CubicBezier(grid->at(3).at(0),grid->at(3).at(1),grid->at(3).at(2),grid->at(3).at(3)));
      
      output->push_back(CubicBezier(grid->at(0).at(0),grid->at(1).at(0),grid->at(2).at(0),grid->at(3).at(0)));
      output->push_back(CubicBezier(grid->at(0).at(1),grid->at(1).at(1),grid->at(2).at(1),grid->at(3).at(1)));
      output->push_back(CubicBezier(grid->at(0).at(2),grid->at(1).at(2),grid->at(2).at(2),grid->at(3).at(2)));
      output->push_back(CubicBezier(grid->at(0).at(3),grid->at(1).at(3),grid->at(2).at(3),grid->at(3).at(3)));
    }

    Point at(float u, float v) {
      Point output(0,0,0);
      float bern_poly1, bern_poly2;
      for (int i = 0; i <= n; i++) {
        bern_poly1 = binomialCoeff.at(i)*pow(u,i)*pow(1-u, n-i);
        for (int j = 0; j <= n; j++) {
          bern_poly2 = binomialCoeff.at(j)*pow(v,j)*pow(1-v, n-j);
          output += bern_poly1*bern_poly2*grid->at(i).at(j);
        }
      }
      return output;
    }

    Vertex vertex_at(float u, float v) {      
      Point v_p0 = CubicBezier(grid->at(0).at(0), grid->at(0).at(1), grid->at(0).at(2), grid->at(0).at(3)).at(u);
      Point v_p1 = CubicBezier(grid->at(1).at(0), grid->at(1).at(1), grid->at(1).at(2), grid->at(1).at(3)).at(u);
      Point v_p2 = CubicBezier(grid->at(2).at(0), grid->at(2).at(1), grid->at(2).at(2), grid->at(2).at(3)).at(u);
      Point v_p3 = CubicBezier(grid->at(3).at(0), grid->at(3).at(1), grid->at(3).at(2), grid->at(3).at(3)).at(u);

      Point u_p0 = CubicBezier(grid->at(0).at(0), grid->at(1).at(0), grid->at(2).at(0), grid->at(3).at(0)).at(v);
      Point u_p1 = CubicBezier(grid->at(0).at(1), grid->at(1).at(1), grid->at(2).at(1), grid->at(3).at(1)).at(v);
      Point u_p2 = CubicBezier(grid->at(0).at(2), grid->at(1).at(2), grid->at(2).at(2), grid->at(3).at(2)).at(v);
      Point u_p3 = CubicBezier(grid->at(0).at(3), grid->at(1).at(3), grid->at(2).at(3), grid->at(3).at(3)).at(v);
 
      CubicBezier vcurve = CubicBezier(v_p0, v_p1, v_p2, v_p3);
      CubicBezier ucurve = CubicBezier(u_p0, u_p1, u_p2, u_p3);
      Point p = vcurve.at(v);

      Vector3f uderiv = ucurve.deriv_at(u);
      Vector3f vderiv = vcurve.deriv_at(v);

      Vector3f normal = uderiv.cross(vderiv) * -1;
      if (normal != Vector3f(0,0,0)) {
        normal.normalize();
      }
      return Vertex(p,normal);
    }

    void getGridPoints(std::vector< std::vector<Vertex> > *output, float precision) {
      for (float u = 0; u <= 1+precision/2; u+=precision) {
        output->push_back(std::vector<Vertex>());
        for (float v = 0; v <= 1+precision/2; v+=precision) {
          output->back().push_back(vertex_at(u,v));
        }
      }
    }

};
#endif
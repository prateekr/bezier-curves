#ifndef BASE_H
#define BASE_H

#include <math.h>

#include <vector>
#include <queue>
#include <Eigen/Dense>

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
};

class BezierPatch {
  public:
    std::vector<std::vector<Point>> *grid;
    std::vector<CubicBezier> *curves;
    std::vector<float> binomialCoeff;
    float n;

    BezierPatch(std::vector<std::vector<Point>> *g) {
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

    Point at2(float u, float v) {
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
};
#endif
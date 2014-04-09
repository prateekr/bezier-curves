#ifndef BASE_H
#define BASE_H

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

class BezierCurve {
  public:
    std::vector<Line> lines;

    BezierCurve() { }

    BezierCurve(std::vector<Point> *list) {
      for (int i = 0; i < list->size() - 1; i++) {
        lines.push_back(Line(list->at(i), list->at(i+1)));
      }
    }

    void getPoints(float precision, std::vector<Point> *line_points) {      
      for (float u = 0; u <= 1.0f; u += precision) {
        std::queue<Line> casteljau_lines;

        for (int i = 1; i < lines.size(); i++) {
          casteljau_lines.push(Line(lines.at(i-1).getPoint(u),lines.at(i).getPoint(u)));
        }

        Line line;

        while (casteljau_lines.size() > 1) {
          Point o = casteljau_lines.front().getPoint(u);
          casteljau_lines.pop();
          Point d = casteljau_lines.front().getPoint(u);
          if (casteljau_lines.size() == 1) {
            line = Line(o, d);
            break;
          }
        }
        line_points->push_back(line.getPoint(u));
      }
    }
};

class CubicBezier : public BezierCurve {
  public:
    CubicBezier(Point p1, Point p2, Point p3, Point p4) {
      lines.push_back(Line(p1, p2));
      lines.push_back(Line(p2, p3));
      lines.push_back(Line(p3, p4));
    }
};

#endif
#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>

#include "base.h"
#include "scene.h"

class Parser {
  public:
    Parser() { }

    void parseFile(std::string fileName, Scene *scene) {
      std::ifstream file(fileName.c_str());
      std::string line;
      std::vector< std::vector<Point> > *points = new std::vector< std::vector<Point> >();

      std::getline(file, line); //Skip first line
      while(std::getline(file, line)) {
        line = std::regex_replace(line, std::regex("^[ ]"), "");
        std::vector<std::string> splitline;
        std::string buf;
        std::stringstream ss(line);

        if (line == "") {
          continue;
        }

        while (ss >> buf) {
          splitline.push_back(buf);
        }

        std::vector<Point> p_row;
        p_row.push_back(Point(atof(splitline.at(0).c_str()), atof(splitline.at(1).c_str()), atof(splitline.at(2).c_str())));
        p_row.push_back(Point(atof(splitline.at(3).c_str()), atof(splitline.at(4).c_str()), atof(splitline.at(5).c_str())));
        p_row.push_back(Point(atof(splitline.at(6).c_str()), atof(splitline.at(7).c_str()), atof(splitline.at(8).c_str())));
        p_row.push_back(Point(atof(splitline.at(9).c_str()), atof(splitline.at(10).c_str()), atof(splitline.at(11).c_str())));
        points->push_back(p_row);
        
        if (points->size() == 4) {
          scene->patches.push_back(new BezierPatch(points));
          points = new std::vector< std::vector<Point> >();
        }
      }
    }
};

#endif
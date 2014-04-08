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

        Point p1 = Point(atof(splitline.at(0).c_str()), atof(splitline.at(1).c_str()), atof(splitline.at(2).c_str()));
        Point p2 = Point(atof(splitline.at(3).c_str()), atof(splitline.at(4).c_str()), atof(splitline.at(5).c_str()));
        Point p3 = Point(atof(splitline.at(6).c_str()), atof(splitline.at(7).c_str()), atof(splitline.at(8).c_str()));
        Point p4 = Point(atof(splitline.at(9).c_str()), atof(splitline.at(10).c_str()), atof(splitline.at(11).c_str()));

        scene->curves.push_back(CubicBezier(p1, p2, p3, p4)); 
      }
    }
};



#endif
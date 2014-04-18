#ifndef OUTPUT_H
#define OUTPUT_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "base.h"
#include "scene.h"

class Output {
  public:
    std::vector<Point> *vertices;
    std::vector<Point> *normals;
    std::vector<Point> *faces; // Encoding the indices of the polygon on the start and end index in the x and y pos of Point
    std::string name;
    int index; // index of the values added to the vectors. Starts at 1

    Output() { }
    Output(std::string name2);

    void exportFile();
};

#endif
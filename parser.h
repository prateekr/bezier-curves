#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "base.h"
#include "scene.h"

class Parser {
  public:
    Parser() { }

    void parseFile(std::string fileName, Scene *scene);
};

#endif
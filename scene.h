#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "base.h"

class Scene {
  public:
    std::vector<BezierPatch*> patches;

    Scene() { }
};

#endif
#ifndef SCENE_H
#define SCENE_H

#include <vector>

class Scene {
  public:
    std::vector<CubicBezier> curves;

    Scene() { }
};

#endif
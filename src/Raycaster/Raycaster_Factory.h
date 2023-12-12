#ifndef GAME_RAYCASTER_FACTORY
#define GAME_RAYCASTER_FACTORY

#include "Raycaster.h"
#include "Raycaster_Step.h"
// #include "Raycaster_DDA.h"
#include <vector>

class Raycaster;
class Raycaster_Step;
// class Raycaster_DDA;
class GameEngine;

enum Raycaster_Type {
  DDA, STEP
};

struct fObj {
  Raycaster * obj;
  Raycaster_Type type;
};

class Raycaster_Factory {
public:
  Raycaster * getInstance(Raycaster_Type type, GameEngine * engine);
private:
  std::vector<fObj> m_objects;
};

Raycaster * Raycaster_Factory::getInstance(Raycaster_Type type, GameEngine * engine) {
  for (auto o : m_objects) if (o.type == type) return o.obj;
  switch (type) {
    case STEP:
      auto inst = std::shared_ptr<Raycaster>(new Raycaster_Step(engine));
      m_objects.push_back({inst.get(), type});
      return inst.get();
  }
}

#endif // GAME_RAYCASTER_FACTORY

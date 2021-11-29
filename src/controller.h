#ifndef CONTROLLER_H
#define CONTROLLER_H

//#include "snake.h"
#include "utils.h"

class Controller {
 public:
  void HandleInput(bool &running, utilities::Click_Message &coordinate) const;

 private:
  void HandleClick(utilities::Click_Message &coordinate, int x, int y) const;
};

#endif
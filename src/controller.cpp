#include "controller.h"
#include <iostream>
#include "SDL.h"

void Controller::HandleClick(utilities::Click_Message &coordinate, int x, int y) const {
  coordinate.coord.x = x;
  coordinate.coord.y = y;
  coordinate.active = true;
  //std::cout << "Coordinates: " << x << " , " << y << "\n";
  //return;
}

void Controller::HandleInput(bool &running, utilities::Click_Message &coordinate) const {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      running = false;
    } else if (e.type == SDL_MOUSEBUTTONDOWN) {
      if(e.button.button == SDL_BUTTON_LEFT && e.button.clicks == 1){
        HandleClick(coordinate, e.button.x, e.button.y);
      }
    }
  }
}